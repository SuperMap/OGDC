OGDC
====

OGDC（Open Geospatial Database Connectivity，开放式空间数据库互访开发标准接口），是基于国家标准《地理空间数据库访问接口》(GB/T30320-2013)的一套C++接口，目的是实现不同格式空间数据库的互联互访。基于 OGDC 标准接口开发的应用程序，不依赖于任何空间数据库及数据格式，以统一的方式来访问空间数据库，并操作二维/三维的点、线、面、栅格及三维模型数据等，从而实现空间数据的互操作。
OGDC 标准接口中提供了统一的接口对空间数据库进行操作。目录结构如下：

（1）Builds ：Visual C++ 11的工程文件，其中：
		Base 工程 ：在 Base 工程中提供了一些基本的几何对象类。主要实现对基本的几何对象进行操作。
		Element 工程 ：在 Element 工程中放置了一些复杂的几何对象相关类。在空间数据库中存储的各种几何对象就被 Element 工程中提供的类进行描述。
		OGDC 工程 ：在 OGDC 工程中则放置了一些数据源、数据集、记录集等相关类。从空间数据库中读取的各种数据就被存储在 OGDC 工程提供的记录集、数据集、数据源类中，并统一进行管理。
（2）Help ：放置了OGDC2.0 的帮助文档。
（3）Include ：OGDC类库的头文件，包含编译与使用各种对象所必须的头文件，以及一个影像插件的头文件 UGImagePlugin.h，其中：
		Element 包含OGDC中使用的二三维空间对象（二维点、线、面、文本，三维点、线、面）的头文件；
		Geometry3D 包含OGDC中使用的模型对象的头文件；
		GeometryCAD 包含OGDC中使用的清华山维对象的头文件；
（4）SampleData ：范例数据。
（5）Samples ：放置了OGDC的范例程序，其中：
		应用层范例程序Viewer：二维点线面文本，三维点线面，清华山维点线面文本的显示。
		应用层范例程序IFCTools：IFC数据通过OGDC接口写入SuperMap的UDB数据源。
		Provider层示范程序 OgdcProviderMDB：通过OGDC接口实现的MDB数据源。
		3ds max模型插件：3ds max场景模型通过OGDC接口写入SuperMap的UDB或直接通过S3MB接口导出S3MB。
（6）license.txt ：最终用户许可协议 。

更多信息参考 http://www.gisforum.net/bbs/index.asp?boardid=118
