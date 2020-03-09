![jackdaw](https://github.com/larrynow/Jackdaw/blob/master/Asset/jackdaw.jpg)
# Jackdaw
基于OPENGL实现的游戏渲染引擎demo。

# 引擎架构  
## 前后端分离的渲染模块
Mesh, Texture等渲染资源在jkContent中统一处理，发送至jkBackendRenderer按不同渲染方式（simple mesh, instance data, cubemap, geometry shader）包装，
供opengl渲染使用。
## 基于Block管理的动态地形
![动态地形](https://github.com/larrynow/Jackdaw/blob/master/Asset/dispaly/display_1.jpg)
动态地形由地形mesh（由高度图动态生成），instance obejects（石块等），几何渲染器（植被），共同实现。根据viewpos动态调整各个block网格精度，环境渲染精细
程度等。
## 基于blinn-Phong光照模型的渲染管线
![渲染效果](https://github.com/larrynow/Jackdaw/blob/master/Asset/dispaly/display_2.jpg)
标准的渲染管线分为以下几个阶段：计算更新VBO，EBO等，设定光源，shadow mapping计算阴影，利用unifrom block，uniform变量更新相关参数，渲染，利用帧缓冲进行后处理（泛光，
伽马矫正）。<br>
具备深度测试、面剔除、混合、立方体贴图、实例化渲染、帧缓冲、离屏MSAA等基本功能。<br>
支持法线贴图，视差贴图，shadow mapping，PCF，HDR。
## 向量、矩阵操作的数学库
简易的数学库：向量计算，LookAt、透视矩阵、平移旋转缩放矩阵的构建。
## 基于Assimp和stbi_load的资源管理模块
利用资源管理模块对Texture，model，shader等进行统一管理。
## 几何体Mesh构建
球形，立方体，平面Mesh的生成。
