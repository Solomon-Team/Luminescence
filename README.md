# 💫 Luminescence

Luminescence is a lightweight Java library providing simple and efficient access to the [Ultralight web renderer](https://ultralig.ht/) via JNI. It's designed to be easy to use and integrate into Java applications, allowing developers to render web content seamlessly.

---
## 🛠 Building From Sources

I'll (hopefully) write proper documentation for this eventually, but basically this is how it goes. First, install the following:
* [IntelliJ IDEA](https://www.jetbrains.com/idea/) (I don't know if other IDEs will work and I probably won't bother testing them. However, if you want to try them out, go ahead. The rest of this guide assumes you're using IntelliJ)
* [CMake](https://cmake.org/download/) (Version 3.20 or higher)
* The build tool corresponding to the operating system you're using:
  * Windows: [Visual Studio 2026](https://visualstudio.microsoft.com/downloads/?q=build+tools) (Doesn't have to be the full IDE, just the build tools are required)
  * MacOS/Linux: GNU Make (Usually comes pre-installed on most Linux distributions and MacOS. If not, you can install it using your package manager)
* JDK 17 or higher (I recommend [Azul Zulu](https://www.azul.com/downloads/#zulu))
* [Ultralight SDK](https://ultralig.ht/download/). (Make sure to download the correct version for your operating system and architecture)

Next, clone the repository:
```bash
git clone https://github.com/Solomon-Team/Luminescence
```

Open the project in IntelliJ and let it import the Gradle project. In the root directory, create a new file named `local.properties` and add the following line to it, replacing the path with the actual path to your Ultralight SDK on disk:
```properties
ultralight_sdk_path=/path/to/ultralight/sdk
```
Now, you should be able to build the project using Gradle. You can do this from the command line by running:
```bash
./gradlew build
```

---

## 📃 License

Luminescence is licensed under the free and open-source license, GNU LGPLv3. For more information, please read the [license](https://choosealicense.com/licenses/lgpl-3.0/).