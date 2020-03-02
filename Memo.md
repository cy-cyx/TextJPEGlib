## 备忘

#### 1、如何导入第三方库

在CMakeList中 *add_library()* 中*STATIC*指静态库.a SHARED*指动态库.so

使用第三方库 *IMPORTED* *set_target_properties()* 具体详见CMakelist

如何使用第三方库使用了是动态的，导入工程，需要导入相应的动态库，使用静态
库就不需要

#### 2、导入相应的头文件

*include_directories()*

#### 3、如何打不同处理器的包

在Gradle中指定 

ndk { // 设置支持的SO库架构 abiFilters 'arm64-v8a' }

#### 4、如果传入是类对象，统一使用jobject

#### 5、在导入第三方注意文件结构

libs
 
 __arm64-v8a