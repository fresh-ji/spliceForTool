
#README

0118：  
（1）使用rapidxml解析配置文件；  
（2）使用json与工具互通数据；  
（3）增加toolTestA和toolTestB工程；  
  
0226：  
（1）动态库start接口增加配置文件参数，配置文件格式为：系统名_模型名.xml  
（2）目前测试系统订阅发布关系：  
    insA发布topic_001，订阅2、3  
    insB发布topic_002，订阅3  
    insC发布topic_002，订阅1，2  
  
目前问题：  
（1）提交时别上传vsproj、filters和sln文件，以防解决不了的冲突；  
（2）json里的constexpr，VC12.0不支持，造成编译不了；  
（3）上个版本toolTestA的设置环境变量注掉了，目前考虑要写到dll里，并且支持一个节点可以 加入到多个域中；
（4）开发引擎工程配合使用；  
