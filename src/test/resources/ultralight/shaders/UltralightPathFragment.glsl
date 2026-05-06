#version 450 core

// ============================================================================
// Ultralight FillPath Fragment Shader — UBO-based uniform access.
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

#define ClipData Uniforms.ClipData
#define Clip     Uniforms.Clip

#define AA_WIDTH 0.354

vec2 transformAffine(vec2 val, vec2 a, vec2 b, vec2 c) { return val.x*a + val.y*b + c; }
void Unpack(vec4 x, out vec4 a, out vec4 b) { const float s=65536.0; a=floor(x/s); b=floor(x-a*s); }
vec4 GetCol(mat4 m, uint i) { return vec4(m[0][i],m[1][i],m[2][i],m[3][i]); }
float antialias2(float d) { return smoothstep(-0.6180469, 0.6180469, d/fwidth(d)); }

float sdRect(vec2 p, vec2 size) { vec2 d=abs(p)-size; return min(max(d.x,d.y),0.0)+length(max(d,vec2(0.0))); }

float sdEllipse(vec2 p, vec2 ab) {
    if (abs(ab.x-ab.y)<0.1) return length(p)-ab.x;
    p=abs(p); if(p.x>p.y){p=p.yx;ab=ab.yx;}
    float l=ab.y*ab.y-ab.x*ab.x,m=ab.x*p.x/l,m2=m*m,n=ab.y*p.y/l,n2=n*n,c=(m2+n2-1.0)/3.0,c3=c*c*c,q=c3+m2*n2*2.0,d=c3+m2*n2,g=m+m*n2,co;
    if(d<0.0){float pp=acos(q/c3)/3.0,s=cos(pp),t=sin(pp)*sqrt(3.0),rx=sqrt(-c*(s+t+2.0)+m2),ry=sqrt(-c*(s-t+2.0)+m2);co=(ry+sign(l)*rx+abs(g)/(rx*ry)-m)/2.0;}
    else{float h=2.0*m*n*sqrt(d),s=sign(q+h)*pow(abs(q+h),1.0/3.0),u=sign(q-h)*pow(abs(q-h),1.0/3.0),rx=-s-u-c*4.0+2.0*m2,ry=(s-u)*sqrt(3.0),rm=sqrt(rx*rx+ry*ry),pp=ry/sqrt(rm-rx);co=(pp+2.0*g/rm-m)/2.0;}
    float si=sqrt(max(1.0-co*co,0.0)); vec2 r=vec2(ab.x*co,ab.y*si); return length(r-p)*sign(p.y-r.y);
}

float sdRoundRect(vec2 p, vec2 size, vec4 rx, vec4 ry) {
    size*=0.5; vec2 corner,local;
    corner=vec2(-size.x+rx.x,-size.y+ry.x); local=p-corner; if(dot(rx.x,ry.x)>0.0&&p.x<corner.x&&p.y<=corner.y) return sdEllipse(local,vec2(rx.x,ry.x));
    corner=vec2(size.x-rx.y,-size.y+ry.y);  local=p-corner; if(dot(rx.y,ry.y)>0.0&&p.x>=corner.x&&p.y<=corner.y) return sdEllipse(local,vec2(rx.y,ry.y));
    corner=vec2(size.x-rx.z,size.y-ry.z);   local=p-corner; if(dot(rx.z,ry.z)>0.0&&p.x>=corner.x&&p.y>=corner.y) return sdEllipse(local,vec2(rx.z,ry.z));
    corner=vec2(-size.x+rx.w,size.y-ry.w);  local=p-corner; if(dot(rx.w,ry.w)>0.0&&p.x<corner.x&&p.y>corner.y)  return sdEllipse(local,vec2(rx.w,ry.w));
    return sdRect(p,size);
}

void applyClip(vec2 objectCoord, inout vec4 outColor) {
    for (uint i=0u; i<uint(ClipData.x); i++) {
        mat4 data=Clip[i];
        vec2 origin=GetCol(data,0u).xy, size=GetCol(data,0u).zw;
        vec4 rx,ry; Unpack(GetCol(data,1u),rx,ry);
        bool inverse=bool(int(GetCol(data,3u).z));
        vec2 p=objectCoord;
        p=transformAffine(p,GetCol(data,2u).xy,GetCol(data,2u).zw,GetCol(data,3u).xy);
        p-=origin;
        float dc=sdRoundRect(p,size,rx,ry)*(inverse?-1.0:1.0);
        float alpha=antialias2(-dc);
        outColor=vec4(outColor.rgb*alpha,outColor.a*alpha);
    }
}

void main()
{
    vec4 col = v_color;
    applyClip(v_obj, col);
    out_Color = col;
}
