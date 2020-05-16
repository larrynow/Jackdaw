![jackdaw](https://github.com/larrynow/Jackdaw/blob/master/Asset/jackdaw.jpg)
# Jackdaw
基于OPENGL实现的游戏渲染引擎demo。

# 引擎架构设计
## 基于单例模式的模块设计
表示当前渲染环境的jkContent类、表示游戏时钟的jkClock类采用了单例设计以提供全局访问入口。
## 前后端分离的渲染模块
Mesh, Texture等渲染资源在jkContent中统一处理，发送至jkBackendRenderer按不同渲染方式（simple mesh, instance data, cubemap, geometry shader）包装，
供opengl渲染使用。不同渲染对象使用一组接口即可方便导入渲染器中进行渲染。
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
同时实现了简易的DeferredShading框架（不完善）。
## 向量、矩阵操作的数学库
简易的数学库：向量计算，LookAt、透视矩阵、平移旋转缩放矩阵的构建。
## 基于Assimp和stbi_load的资源管理模块
利用智能指针对Texture，model，shader等进行统一导入管理。
## 几何体Mesh构建
球形，立方体，平面Mesh的生成。
## 骨骼动画的计算渲染
骨骼动画以动画结点组成的树状结构表示，动画播放时以递归方式更新骨骼矩阵，在GL渲染管线中渲染动画。
## 游戏Object的继承体系
分别以jkEntity表示静态物体，jkActor表示可移动物体，jkCharacter表示可控物体。
## jkTransform组件管理形态数据
游戏物体以单独的jkTransform组件，管理实体的方向，位置，缩放信息。
## State设计模式更新物体状态
使用了有限状态机判断更新物体的状态。
## 系统输入处理
系统输入被包装为jkInput的enum class，在SysInputManager中更新输入状态。在jkCharacter类中注册过的输入可以在各个状态中处理，并进行状态转换等。
