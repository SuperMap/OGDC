OGDC
====

OGDC，全称为 Open Geospatial Database Connectivity，中文名称为开放式空间数据库互访开发标准接口，是为了实现不同格式空间数据库互联互访而基于 GDB-CLI 标准的开放式空间数据库互访开发标准接口。基于 OGDC 标准接口开发的应用程序，不依赖于任何空间数据库及数据格式，以统一的方式来访问所有的空间数据，实现对空间数据的读写，从而达到空间数据的互操作。OGDC 标准接口中提供了统一的接口对空间数据库进行操作。目录结构如下：

（1）Builds ：Visual C++ 6和Visual C++ 9的工程文件，其中：
		Base 工程 ：在 Base 工程中提供了一些基本的几何对象类。主要实现对基本的几何对象进行操作。
		Element 工程 ：在 Element 工程中放置了一些复杂的几何对象相关类。在空间数据库中存储的各种几何对象就被 Element 工程中提供的类进行描述。
		OGDC 工程 ：在 OGDC 工程中则放置了一些数据源、数据集、记录集等相关类。从空间数据库中读取的各种数据就被存储在 OGDC 工程提供的记录集、数据集、数据源类中，并统一进行管理。
（2）Help ：放置了OGDC 的帮助文档和产品入门 的帮助文档和产品入门 文档 。
（3）Include ：OGDC类库的头文件，包含 Base、Elemen、OGDC 三个工程的头文件，以及一个影像插件的头文件 UGImagePlugin.h。
（4）LibSharedLibShared ：放置了OGDC的 Visual C++ 6版本需要的第三方依赖库。
（5）SampleData ：范例数据。
（6）Samples ：放置了OGDC的范例程序，包括一个应用层范例程序Viewer和一个Provider层示范程序 OgdcProviderMDB。
（7）license.txt ：最终用户许可协议 。

更多信息参考 http://www.gisforum.net/bbs/index.asp?boardid=118
