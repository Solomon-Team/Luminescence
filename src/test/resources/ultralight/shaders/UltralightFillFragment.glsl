#version 450 core

// ============================================================================
// Ultralight Fill Fragment Shader — UBO-based uniform access.
// All uniforms are read from the type_Uniforms block at binding 0.
// ============================================================================

in vec4 v_color;
in vec2 v_tex;
in vec2 v_obj;
in vec4 v_data0;
in vec4 v_data1;
in vec4 v_data2;
in vec4 v_data3;
in vec4 v_data4;
in vec4 v_data5;
in vec4 v_data6;

out vec4 out_Color;

layout(std140, binding = 0) uniform type_Uniforms
{
    vec4  State;
    mat4  Transform;
    ivec4 Integer4[2];
    vec4  Scalar4[2];
    vec4  Vector[8];
    ivec4 ClipData;
    mat4  Clip[8];
} Uniforms;

uniform sampler2D SPIRV_Cross_CombinedTexture0Sampler0;
uniform sampler2D SPIRV_Cross_CombinedTexture1Sampler0;
uniform sampler2D SPIRV_Cross_CombinedTexture2Sampler0;

// Convenience accessors
#define Scalar4     Uniforms.Scalar4
#define Vector      Uniforms.Vector
#define ClipData    Uniforms.ClipData
#define Clip        Uniforms.Clip

float Scalar(int i) { return (i < 4) ? Scalar4[0][i] : Scalar4[1][i - 4]; }

// ---- Clip helpers -----------------------------------------------------------

#define AA_WIDTH 0.354

vec2 transformAffine(vec2 val, vec2 a, vec2 b, vec2 c) {
    return val.x * a + val.y * b + c;
}

void Unpack(vec4 x, out vec4 a, out vec4 b) {
    const float s = 65536.0;
    a = floor(x / s);
    b = floor(x - a * s);
}

vec4 GetCol(mat4 m, uint i) {
    return vec4(m[0][i], m[1][i], m[2][i], m[3][i]);
}

float antialias(float d, float width, float median) {
    return smoothstep(median - width, median + width, d);
}

float antialias2(float d) {
    return smoothstep(-0.6180469, 0.6180469, d / fwidth(d));
}

float sdRect(vec2 p, vec2 size) {
    vec2 d = abs(p) - size;
    return min(max(d.x, d.y), 0.0) + length(max(d, vec2(0.0)));
}

float sdEllipse(vec2 p, vec2 ab) {
    if (abs(ab.x - ab.y) < 0.1) return length(p) - ab.x;
    p = abs(p);
    if (p.x > p.y) { p = p.yx; ab = ab.yx; }
    float l = ab.y*ab.y - ab.x*ab.x;
    float m = ab.x*p.x/l; float m2 = m*m;
    float n = ab.y*p.y/l; float n2 = n*n;
    float c = (m2+n2-1.0)/3.0; float c3 = c*c*c;
    float q = c3 + m2*n2*2.0;
    float d = c3 + m2*n2;
    float g = m + m*n2;
    float co;
    if (d < 0.0) {
        float pp = acos(q/c3)/3.0;
        float s = cos(pp), t = sin(pp)*sqrt(3.0);
        float rx = sqrt(-c*(s+t+2.0)+m2);
        float ry = sqrt(-c*(s-t+2.0)+m2);
        co = (ry + sign(l)*rx + abs(g)/(rx*ry) - m)/2.0;
    } else {
        float h = 2.0*m*n*sqrt(d);
        float s = sign(q+h)*pow(abs(q+h),1.0/3.0);
        float u = sign(q-h)*pow(abs(q-h),1.0/3.0);
        float rx = -s-u-c*4.0+2.0*m2;
        float ry = (s-u)*sqrt(3.0);
        float rm = sqrt(rx*rx+ry*ry);
        float pp = ry/sqrt(rm-rx);
        co = (pp+2.0*g/rm-m)/2.0;
    }
    float si = sqrt(max(1.0-co*co,0.0));
    vec2 r = vec2(ab.x*co, ab.y*si);
    return length(r-p)*sign(p.y-r.y);
}

float sdRoundRect(vec2 p, vec2 size, vec4 rx, vec4 ry) {
    size *= 0.5;
    vec2 corner;
    corner = vec2(-size.x+rx.x,-size.y+ry.x);
    vec2 local = p - corner;
    if (dot(rx.x,ry.x)>0.0 && p.x<corner.x && p.y<=corner.y) return sdEllipse(local,vec2(rx.x,ry.x));
    corner = vec2(size.x-rx.y,-size.y+ry.y); local = p - corner;
    if (dot(rx.y,ry.y)>0.0 && p.x>=corner.x && p.y<=corner.y) return sdEllipse(local,vec2(rx.y,ry.y));
    corner = vec2(size.x-rx.z,size.y-ry.z); local = p - corner;
    if (dot(rx.z,ry.z)>0.0 && p.x>=corner.x && p.y>=corner.y) return sdEllipse(local,vec2(rx.z,ry.z));
    corner = vec2(-size.x+rx.w,size.y-ry.w); local = p - corner;
    if (dot(rx.w,ry.w)>0.0 && p.x<corner.x && p.y>corner.y) return sdEllipse(local,vec2(rx.w,ry.w));
    return sdRect(p,size);
}

float innerStroke(float stroke_width, float d) {
    return min(antialias(-d, AA_WIDTH, 0.0), 1.0 - antialias(-d, AA_WIDTH, stroke_width));
}

float ramp(float inMin, float inMax, float val) {
    return clamp((val - inMin) / (inMax - inMin), 0.0, 1.0);
}

void applyClip(vec2 objectCoord, inout vec4 outColor) {
    for (uint i = 0u; i < uint(ClipData.x); i++) {
        mat4 data    = Clip[i];
        vec2 origin  = GetCol(data,0u).xy;
        vec2 size    = GetCol(data,0u).zw;
        vec4 radii_x, radii_y;
        Unpack(GetCol(data,1u), radii_x, radii_y);
        bool inverse = bool(int(GetCol(data,3u).z));
        vec2 p = objectCoord;
        p = transformAffine(p, GetCol(data,2u).xy, GetCol(data,2u).zw, GetCol(data,3u).xy);
        p -= origin;
        float d_clip = sdRoundRect(p, size, radii_x, radii_y) * (inverse ? -1.0 : 1.0);
        float alpha = antialias2(-d_clip);
        outColor = vec4(outColor.rgb * alpha, outColor.a * alpha);
    }
}

// ---- Fill type accessors ----------------------------------------------------

uint  FillType()           { return uint(v_data0.x + 0.5); }
uint  Gradient_NumStops()  { return uint(v_data0.y + 0.5); }
bool  Gradient_IsRadial()  { return bool(uint(v_data0.z + 0.5)); }
vec2  Gradient_P0()        { return v_data1.xy; }
vec2  Gradient_P1()        { return v_data1.zw; }
vec2  Gradient_R0()        { return v_data2.xy; }
vec2  Gradient_R1()        { return v_data2.zw; }

struct GradientStop { float percent; vec4 color; };

GradientStop GetGradientStop(uint offset) {
    GradientStop s;
    if (offset < 3u) {
        s.percent = v_data3[offset];
        if      (offset == 0u) s.color = v_data4;
        else if (offset == 1u) s.color = v_data5;
        else                   s.color = v_data6;
    } else {
        s.percent = Scalar(int(offset) - 3);
        s.color   = Vector[int(offset) - 3];
    }
    return s;
}

// ---- Fill implementations ---------------------------------------------------

vec4 fillSolid()        { return v_color; }
vec4 fillImage()        { return texture(SPIRV_Cross_CombinedTexture0Sampler0, v_tex) * v_color; }

vec4 fillPatternImage() {
    vec4 tileRectUV    = Vector[0];
    vec2 spacing       = Vector[1].xy;
    vec2 tileSize      = Vector[1].zw;
    vec2 logicalSize   = tileSize + spacing;
    vec2 transA        = Vector[2].xy;
    vec2 transB        = Vector[2].zw;
    vec2 transC        = Vector[3].xy;
    vec2 p = v_obj;
    vec2 coords = p.x * transA + p.y * transB + transC;
    coords /= logicalSize;
    vec2 wrapped       = fract(coords);
    vec2 pixelInTile   = wrapped * logicalSize;
    if (pixelInTile.x >= tileSize.x || pixelInTile.y >= tileSize.y) return vec4(0.0);
    vec2 uv = pixelInTile / tileSize;
    uv = uv * (tileRectUV.zw - tileRectUV.xy) + tileRectUV.xy;
    return texture(SPIRV_Cross_CombinedTexture0Sampler0, uv) * v_color;
}

vec4 fillPatternGradient() {
    uint num_stops = Gradient_NumStops();
    bool is_radial = Gradient_IsRadial();
    vec2 p0 = Gradient_P0(), p1 = Gradient_P1();
    vec2 r0 = Gradient_R0(), r1 = Gradient_R1();
    float t = 0.0;
    vec2 coord = v_tex;
    if (is_radial) {
        vec2 delta = coord - p0;
        vec2 dOuter = delta / r1;
        float distOuter = length(dOuter);
        if (r0.x > 0.0001 && r0.y > 0.0001) {
            vec2 dInner = delta / r0;
            float distInner = length(dInner);
            t = (distInner <= 1.0) ? 0.0 : (distOuter >= 1.0) ? 1.0 :
            clamp((distInner-1.0)/(distInner-distOuter),0.0,1.0);
        } else { t = clamp(distOuter,0.0,1.0); }
        if (length(p1-p0) > 0.0001) {
            vec2 fd = normalize(p0-p1), pd = normalize(coord-p1);
            t = clamp(t*(1.0+(1.0-dot(pd,fd))*0.5),0.0,1.0);
        }
    } else {
        vec2 V = p1 - p0;
        t = clamp(dot(coord-p0,V)/dot(V,V),0.0,1.0);
    }

    GradientStop sPrev = GetGradientStop(0u);
    GradientStop sCurr = GetGradientStop(1u);
    vec4 col = mix(sPrev.color, sCurr.color, ramp(sPrev.percent, sCurr.percent, t));

    if (num_stops > 2u) { sPrev = sCurr; sCurr = GetGradientStop(2u); col = mix(col, sCurr.color, ramp(sPrev.percent, sCurr.percent, t)); }
    if (num_stops > 3u) { sPrev = sCurr; sCurr = GetGradientStop(3u); col = mix(col, sCurr.color, ramp(sPrev.percent, sCurr.percent, t)); }
    if (num_stops > 4u) { sPrev = sCurr; sCurr = GetGradientStop(4u); col = mix(col, sCurr.color, ramp(sPrev.percent, sCurr.percent, t)); }
    if (num_stops > 5u) { sPrev = sCurr; sCurr = GetGradientStop(5u); col = mix(col, sCurr.color, ramp(sPrev.percent, sCurr.percent, t)); }
    if (num_stops > 6u) { sPrev = sCurr; sCurr = GetGradientStop(6u); col = mix(col, sCurr.color, ramp(sPrev.percent, sCurr.percent, t)); }
    if (num_stops > 7u) { sPrev = sCurr; sCurr = GetGradientStop(7u); col = mix(col, sCurr.color, ramp(sPrev.percent, sCurr.percent, t)); }
    if (num_stops > 8u) { sPrev = sCurr; sCurr = GetGradientStop(8u); col = mix(col, sCurr.color, ramp(sPrev.percent, sCurr.percent, t)); }
    if (num_stops > 9u) { sPrev = sCurr; sCurr = GetGradientStop(9u); col = mix(col, sCurr.color, ramp(sPrev.percent, sCurr.percent, t)); }
    if (num_stops > 10u){ sPrev = sCurr; sCurr = GetGradientStop(10u);col = mix(col, sCurr.color, ramp(sPrev.percent, sCurr.percent, t)); }

    return col;
}

vec4 blend_pd(vec4 src, vec4 dest) {
    return vec4(src.rgb + dest.rgb*(1.0-src.a), src.a + dest.a*(1.0-src.a));
}

vec4 fillRoundedRect() {
    vec2 p    = v_tex;
    vec2 size = v_data0.zw;
    p = (p - 0.5) * size;
    float d = sdRoundRect(p, size, v_data1, v_data2);
    float alpha = antialias(-d, AA_WIDTH, 0.0) * v_color.a;
    vec4 col = vec4(v_color.rgb * alpha, alpha);
    float sw = v_data3.x;
    if (sw > 0.0) {
        vec4 sc = v_data4;
        float sa = innerStroke(sw, d) * sc.a;
        col = blend_pd(vec4(sc.rgb*sa,sa), col);
    }
    return col;
}

vec4 fillBoxShadow() { return v_color; }

vec3 blendOverlay(vec3 s,vec3 d){ vec3 c; for(int i=0;i<3;i++) c[i]=d[i]<0.5?2.0*d[i]*s[i]:1.0-2.0*(1.0-d[i])*(1.0-s[i]); return c; }
vec3 blendColorDodge(vec3 s,vec3 d){ vec3 c; for(int i=0;i<3;i++) c[i]=(s[i]==1.0)?1.0:min(d[i]/(1.0-s[i]),1.0); return c; }
vec3 blendColorBurn(vec3 s,vec3 d){ vec3 c; for(int i=0;i<3;i++) c[i]=(s[i]==0.0)?0.0:max(1.0-((1.0-d[i])/s[i]),0.0); return c; }
vec3 blendSoftLight(vec3 s,vec3 d){ vec3 c; for(int i=0;i<3;i++) c[i]=s[i]<0.5?2.0*d[i]*s[i]+d[i]*d[i]*(1.0-2.0*s[i]):sqrt(d[i])*(2.0*s[i]-1.0)+2.0*d[i]*(1.0-s[i]); return c; }
vec3 rgb2hsl(vec3 c){ const float e=1e-7; float mn=min(c.r,min(c.g,c.b)),mx=max(c.r,max(c.g,c.b)); vec3 mask=step(c.grr,c.rgb)*step(c.bbg,c.rgb); vec3 h=mask*(vec3(0.0,2.0,4.0)+(c.gbr-c.brg)/(mx-mn+e))/6.0; return vec3(fract(1.0+h.x+h.y+h.z),(mx-mn)/(1.0-abs(mn+mx-1.0)+e),(mn+mx)*0.5); }
vec3 hsl2rgb(vec3 c){ vec3 rgb=clamp(abs(mod(c.x*6.0+vec3(0.0,4.0,2.0),6.0)-3.0)-1.0,0.0,1.0); return c.z+c.y*(rgb-0.5)*(1.0-abs(2.0*c.z-1.0)); }
vec3 blendHue(vec3 s,vec3 d)       { vec3 b=rgb2hsl(d); return hsl2rgb(vec3(rgb2hsl(s).r,b.g,b.b)); }
vec3 blendSaturation(vec3 s,vec3 d){ vec3 b=rgb2hsl(d); return hsl2rgb(vec3(b.r,rgb2hsl(s).g,b.b)); }
vec3 blendColor(vec3 s,vec3 d)     { vec3 b=rgb2hsl(s); return hsl2rgb(vec3(b.r,b.g,rgb2hsl(d).b)); }
vec3 blendLuminosity(vec3 s,vec3 d){ vec3 b=rgb2hsl(d); return hsl2rgb(vec3(b.r,b.g,rgb2hsl(s).b)); }

vec4 fillBlend() {
    vec4 src  = fillImage();
    vec4 dest = texture(SPIRV_Cross_CombinedTexture1Sampler0, v_obj);
    uint op = uint(v_data0.y + 0.5);
    if      (op==0u) return vec4(0.0);
    else if (op==1u) return src;
    else if (op==2u) return src + dest*(1.0-src.a);
    else if (op==3u) return src*dest.a;
    else if (op==4u) return src*(1.0-dest.a);
    else if (op==5u) return src*dest.a + dest*(1.0-src.a);
    else if (op==6u) return src*(1.0-dest.a)+dest;
    else if (op==7u) return dest*src.a;
    else if (op==8u) return dest*(1.0-src.a);
    else if (op==9u) return src*(1.0-dest.a)+dest*src.a;
    else if (op==10u) return clamp(src*(1.0-dest.a)+dest*(1.0-src.a),0.0,1.0);
    else if (op==11u) return vec4(min(src.rgb,dest.rgb)*src.a,dest.a*src.a);
    else if (op==12u) return clamp(src+dest,0.0,1.0);
    else if (op==13u) return vec4(abs(dest.rgb-src.rgb)*src.a,dest.a*src.a);
    else if (op==14u) return vec4(src.rgb*dest.rgb*src.a,dest.a*src.a);
    else if (op==15u) return vec4((1.0-((1.0-dest.rgb)*(1.0-src.rgb)))*src.a,dest.a*src.a);
    else if (op==16u) return vec4(blendOverlay(src.rgb,dest.rgb)*src.a,dest.a*src.a);
    else if (op==17u) return vec4(max(src.rgb,dest.rgb)*src.a,dest.a*src.a);
    else if (op==18u) return vec4(blendColorDodge(src.rgb,dest.rgb)*src.a,dest.a*src.a);
    else if (op==19u) return vec4(blendColorBurn(src.rgb,dest.rgb)*src.a,dest.a*src.a);
    else if (op==20u) return vec4(blendOverlay(dest.rgb,src.rgb)*src.a,dest.a*src.a);
    else if (op==21u) return vec4(blendSoftLight(src.rgb,dest.rgb)*src.a,dest.a*src.a);
    else if (op==22u) return vec4((dest.rgb+src.rgb-2.0*dest.rgb*src.rgb)*src.a,dest.a*src.a);
    else if (op==23u) return vec4(blendHue(src.rgb,dest.rgb)*src.a,dest.a*src.a);
    else if (op==24u) return vec4(blendSaturation(src.rgb,dest.rgb)*src.a,dest.a*src.a);
    else if (op==25u) return vec4(blendColor(src.rgb,dest.rgb)*src.a,dest.a*src.a);
    else if (op==26u) return vec4(blendLuminosity(src.rgb,dest.rgb)*src.a,dest.a*src.a);
    return src;
}

vec4 fillMask() {
    vec4 col = fillImage();
    float a  = texture(SPIRV_Cross_CombinedTexture1Sampler0, v_obj).a;
    return vec4(col.rgb*a, col.a*a);
}

float AlphaCorrection(float a, float l) {
    if (a < 0.866667) {
        float a2=a*a,a3=a2*a,a4=a3*a,l2=l*l,l3=l2*l,l4=l3*l;
        float p=-0.5295142877+0.6729336378*l-0.9970081304*a-6.9761895120*l2-1.2416343696*a*l+8.0153698029*a2+52.6805538770*l3-23.9237999543*a*l2+26.8822958232*a2*l-31.4318707953*a3-72.3097846202*l4+7.1798346847*a*l3+22.1490959100*a2*l2-44.2197918196*a3*l+45.1474487610*a4+29.2352950634*pow(l,5.0)+3.7994888864*a*l4-5.7742577614*a2*l3-12.0929004495*a3*l2+27.8164399735*a4*l-23.5607835614*pow(a,5.0);
        return clamp(a + a*(1.0-a)*p, 0.0, 1.0);
    } else {
        float a2=a*a,a3=a2*a,a4=a3*a,l2=l*l,l3=l2*l,l4=l3*l;
        float p=359.7440629041-192.8896711219*l-856.5769056815*a-309.4254650715*l2+1099.4938500108*a*l+75.8802629888*a2-82.2700940268*l3+837.4432826429*a*l2-1796.8031907127*a2*l+1033.9126625702*a3-2.5168630999*l4+98.2029223745*a*l3-559.1591121667*a2*l2+914.3739143865*a3*l-619.1996126173*a4;
        return clamp(a + a*(1.0-a)*p, 0.0, 1.0);
    }
}

vec4 fillGlyph() {
    float alpha = texture(SPIRV_Cross_CombinedTexture0Sampler0, v_tex).a;
    float luma  = v_data0.y;
    float ca    = AlphaCorrection(alpha, luma) * v_color.a;
    return vec4(v_color.rgb * ca, ca);
}

void main()
{
    const uint FT_Solid    = 0u;
    const uint FT_Image    = 1u;
    const uint FT_PatImg   = 2u;
    const uint FT_PatGrad  = 3u;
    const uint FT_RndRect  = 7u;
    const uint FT_BoxShadow= 8u;
    const uint FT_Blend    = 9u;
    const uint FT_Mask     = 10u;
    const uint FT_Glyph    = 11u;

    uint ft = FillType();
    vec4 col;
    if      (ft == FT_Solid)     col = fillSolid();
    else if (ft == FT_Image)     col = fillImage();
    else if (ft == FT_PatImg)    col = fillPatternImage();
    else if (ft == FT_PatGrad)   col = fillPatternGradient();
    else if (ft == FT_RndRect)   col = fillRoundedRect();
    else if (ft == FT_BoxShadow) col = fillBoxShadow();
    else if (ft == FT_Blend)     col = fillBlend();
    else if (ft == FT_Mask)      col = fillMask();
    else if (ft == FT_Glyph)     col = fillGlyph();
    else                         col = v_color;

    applyClip(v_obj, col);
    out_Color = col;
}