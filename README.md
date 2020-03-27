
#README

（1）使用rapidxml解析配置文件；
（2）使用rapidjson与工具互通数据；
（3）增加spdlog管理日志；
（4）测试系统订阅发布关系：
    insA发布topic_001，订阅2、3
    insB发布topic_002，订阅3
    insC发布topic_002，订阅1，2