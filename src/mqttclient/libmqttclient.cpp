/**
  ******************************************************************************
  * @file    libmqttclient.cpp
  * @author  aron566
  * @version v1.0
  * @date    2020-08-27
  * @brief   Mqtt客户端.
  ******************************************************************************
  */
/* Header includes -----------------------------------------------------------*/
#include "libmqttclient.h"
#include "ui_libmqttclient.h"
#include <QMessageBox>
#include <QTextCodec>
#include <QDateTime>
#include "customerwidgetitem.h"
/* Macro definitions ---------------------------------------------------------*/
#define HOST_NAME_CHANGED       0x0001U /**< 域名已修改*/
#define SERVER_IP_CHANGED       0x0002U /**< IP已修改*/
#define SERVER_PORT_CHANGED     0x0004U /**< 端口已修改*/
#define USER_NAME_CHANGED       0x0008U /**< 用户名已修改*/
#define USER_KEY_CHANGED        0x0010U /**< 密码已修改*/
#define SUBSCRIBE_CHANGED       0x0020U /**< 订阅已修改*/
#define SUBSCRIBE_QOS_CHANGED   0x0040U /**< 订阅服务质量已修改*/
#define PUBLISH_CHANGED         0x0080U /**< 发布已修改*/
#define PUBLISH_QOS_CHANGED     0x0100U /**< 发布服务质量已修改*/
/**
 * QoS0，最多一次送达。也就是发出去就fire掉，没有后面的事情了。
 * QoS1，至少一次送达。发出去之后必须等待ack，没有ack，就要找时机重发
 * QoS2，准确一次送达。消息id将拥有一个简单的生命周期
 */
#if defined(Q_CC_MINGW)
#define MQTT_JSON_FILE "MQTT_CONFIG.json"
#else
#define MQTT_JSON_FILE "/etc/MQTT_CONFIG.json"
#endif

#define MQTT_CLIENT_MAX_NUM     4U
/* Type definitions ----------------------------------------------------------*/
/* Variable declarations -----------------------------------------------------*/
/* Variable definitions ------------------------------------------------------*/
QString Libmqttclient::MqttClentID = QString::fromUtf8("Client_0");
QString Libmqttclient::MqttServerAddr = QString::fromUtf8("broker.hivemq.com");
QString Libmqttclient::MqttServerIp = QString::fromUtf8("192.168.50.128");
quint16 Libmqttclient::MqttServerPort = 1883;
QString Libmqttclient::MqttUserName = QString::fromUtf8("admin");
QString Libmqttclient::MqttUserKey = QString::fromUtf8("123456");
quint16 Libmqttclient::MqttKeepAlive = 300;
QStringList Libmqttclient::MqttSubscribeTopic;
QList<qint32>Libmqttclient::MqttSUBQos;
QStringList Libmqttclient::MqttPublishTopic;
QList<qint32>Libmqttclient::MqttPUBQos;

/*label样式*/
const QString m_red_SheetStyle = QString::fromUtf8("min-width: 30px; min-height: 30px;max-width:30px; max-height: 30px;border-radius: 15px;  border:1px solid black;background:red");

const QString m_green_SheetStyle = QString::fromUtf8("min-width: 30px; min-height: 30px;max-width:30px; max-height: 30px;border-radius: 15px;  border:1px solid black;background:green");

const QString m_grey_SheetStyle = QString::fromUtf8("min-width: 30px; min-height: 30px;max-width:30px; max-height: 30px;border-radius: 15px;  border:1px solid black;background:grey");

const QString m_yellow_SheetStyle = QString::fromUtf8("min-width: 30px; min-height: 30px;max-width:30px; max-height: 30px;border-radius: 15px;  border:1px solid black;background:yellow");

/*字体样式*/
const QString font_suffix = QString::fromUtf8("</font>");
const QString red_prefix = QString::fromUtf8("<font color='red'>");

const QString blue_prefix = QString::fromUtf8("<font color='blue'>");

const QString green_prefix = QString::fromUtf8("<font color='green'>");

const QString white_prefix = QString::fromUtf8("<font color='white'>");

const QString orange_prefix = QString::fromUtf8("<font color='orange'>");

/*对话框样式*/
const QString dialog_suffix = QString::fromUtf8("</span></p>");
const QString dialog_pingk_prefix = QString::fromUtf8("<p><span style='color: rgb(255, 0, 80);font-size: 24px;'>");

/* Function declarations -----------------------------------------------------*/
/* Function definitions ------------------------------------------------------*/
/**
 * @brief Libmqttclient::mqtt
 * @param parent
 */
Libmqttclient::Libmqttclient(int width ,int height ,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::libmqttclient)
{
    /*设置窗口大小*/
    ui->setWINDOWsize(width ,height);

    /*初始化窗口*/
    ui->setupUi(this);

    /*去掉windows关闭窗口按钮*/
    this->setWindowFlag(Qt::FramelessWindowHint);

    /*初始隐藏窗口*/
    this->hide();

    /*关闭调试模式*/
    CloseDebugMode();

    /*创建自启动网络检测定时器*/
    NetworkCheckTimer = new QTimer(this);
    NetworkCheckTimer->setInterval(5000);
    connect(NetworkCheckTimer ,&QTimer::timeout ,this ,&Libmqttclient::DealTimeout);

    /*重连定时器*/
    ReconnectTimer = new QTimer(this);
    ReconnectTimer->setInterval(10000);
    connect(ReconnectTimer ,&QTimer::timeout ,this ,&Libmqttclient::DealDisconnectTimeout);

    /*网络检测结果搜集*/
    connect(&Exec ,&QProcess::readyReadStandardOutput, this ,&Libmqttclient::slotexeFinished);

    /*参数初始化*/
    ConfigParInit();

    /*标志位初始化*/
    ConfigFlagInit();

    /*mqtt初始化*/
    MqttConfigInit();
}

/**
 * @brief Libmqttclient::mqtt
 * @param ip ip地址
 * @param parent
 * @param addr 域名
 * @param username 用户名
 * @param key 密码
 * @param port 端口
 * @param keepalive 保活时间
 */
Libmqttclient::Libmqttclient(int width ,int height ,QString ip ,bool usessl ,quint8 clientnum ,QWidget *parent ,QString addr ,QString username ,QString key ,quint16 port ,quint16 keepalive)
    : QWidget(parent)
    ,ui(new Ui::libmqttclient)
    ,UseSSL(usessl)
{
    /*设置窗口大小*/
    ui->setWINDOWsize(width ,height);

    /*初始化窗口*/
    ui->setupUi(this);

    /*去掉windows关闭窗口按钮*/
    this->setWindowFlag(Qt::FramelessWindowHint);

    /*初始隐藏窗口*/
    this->hide();

    /*关闭调试模式*/
    CloseDebugMode();

    /*创建自启动网络检测定时器*/
    NetworkCheckTimer = new QTimer();
    NetworkCheckTimer->setInterval(5000);
    connect(NetworkCheckTimer ,&QTimer::timeout ,this ,&Libmqttclient::DealTimeout);

    /*重连定时器*/
    ReconnectTimer = new QTimer();
    ReconnectTimer->setInterval(5000);
    connect(ReconnectTimer ,&QTimer::timeout ,this ,&Libmqttclient::DealDisconnectTimeout);

    /*网络检测结果搜集*/
    connect(&Exec ,&QProcess::readyReadStandardOutput, this ,&Libmqttclient::slotexeFinished);

    /*参数预设*/
    PreSetConfig(clientnum ,addr ,ip ,username ,key ,port ,keepalive);

    /*参数初始化*/
    ConfigParInit();

    /*标志位初始化*/
    ConfigFlagInit();

    /*mqtt初始化*/
    MqttConfigInit();
}
/**
 * @brief Libmqttclient::~mqtt
 */
Libmqttclient::~Libmqttclient()
{
    delete ui;
}

/**
 * @brief Libmqttclient::PreSetConfig
 * @param addr
 * @param ip
 * @param username
 * @param key
 * @param port
 * @param keepalive
 */
void Libmqttclient::PreSetConfig(quint8 clientnum ,QString addr ,QString ip ,QString username ,QString key ,quint16 port ,quint16 keepalive)
{
    MqttServerAddr = addr;
    MqttServerIp = ip;
    MqttServerPort = port;
    MqttUserName = username;
    MqttUserKey = key;
    MqttKeepAlive = keepalive;
    ActiveClientNum = clientnum;
    /*更新网络参数标题*/
    ui->NETWOEKgroupBox->setTitle(QString("网络%1连接参数").arg(ActiveClientNum));
}

/**
 * @brief Libmqttclient::ConfigParInit
 */
void Libmqttclient::ConfigParInit()
{
    QJsonObject ConfigrootObj;
    QFile file(MQTT_JSON_FILE);

    /*检测配置文件是否存在，不存在则创建否则读取参数*/
    if(file.exists() == false)
    {
        Create_ConfigFile();
    }
    else
    {
        ReadConfigFromJson(ConfigrootObj);
    }
}

/**
 * @brief Libmqttclient::Create_ConfigFile
 */
void Libmqttclient::Create_ConfigFile()
{
    /*写入权限打开文件*/
    QFile file(MQTT_JSON_FILE);
    if(!file.open(QIODevice::WriteOnly)) {
        qDebug() << "File open failed!";
    }

    QJsonObject ConfigrootObj;
    /*循环添加参数进入json数组*/
    QJsonObject MqttConfigObj;
    QJsonArray classArray;
    QJsonArray subclassArray;
    QJsonObject subobj;
    QJsonArray pubclassArray;
    QJsonObject pubobj;
    for(quint8 i = 0;i < MQTT_CLIENT_MAX_NUM; i++)
    {
        /*重置参数*/
        MqttSubscribeTopic.clear();
        MqttSUBQos.clear();
        MqttPublishTopic.clear();
        MqttPUBQos.clear();
        if(subclassArray.isEmpty() == false)
        {
            subclassArray.removeLast();
        }
        if(pubclassArray.isEmpty() == false)
        {
            pubclassArray.removeLast();
        }

        MqttConfigObj["MqttConfigNum"] = QString("Client_%1").arg(i);
        MqttConfigObj["MqttServerAddr"] = MqttServerAddr;
        MqttConfigObj["MqttServerIp"] = MqttServerIp;
        MqttConfigObj["MqttServerPort"] = MqttServerPort;
        MqttConfigObj["MqttUserName"] = MqttUserName;
        MqttConfigObj["MqttUserKey"] = MqttUserKey;
        MqttConfigObj["MqttKeepAlive"] = MqttKeepAlive;

        /*加入订阅列表*/
        MqttSubscribeTopic.append("");
        subobj["MqttSubscribeTopic"] = MqttSubscribeTopic.value(i);
        MqttSUBQos.append(0);
        subobj["MqttSUBQos"] = MqttSUBQos.value(i);
        subclassArray.append(subobj);
        MqttConfigObj["SubscribeTopic"] = subclassArray;

        /*加入发布列表*/
        MqttPublishTopic.append("");
        pubobj["MqttPublishTopic"] = MqttPublishTopic.value(i);
        MqttPUBQos.append(0);
        pubobj["MqttPUBQos"] = MqttPUBQos.value(i);
        pubclassArray.append(pubobj);
        MqttConfigObj["PublishTopic"] = pubclassArray;

        classArray.append(MqttConfigObj);
    }

    /*窗口配置数组加入json*/
    ConfigrootObj["MQTT_CONFIG"] = classArray;

    /*转换并写入*/
    QJsonDocument jdoc(ConfigrootObj);
    file.write(jdoc.toJson());
    file.flush();
    file.close();
}

/**
 * @brief Libmqttclient::ReadConfigFromJson
 * @param rootObj
 * @return true读取成功
 */
bool Libmqttclient::ReadConfigFromJson(QJsonObject &rootObj ,bool updateConfig)
{
    QFile file(MQTT_JSON_FILE);

    /*检测文件是否存在*/
    if(file.exists() == false)
    {
        ConfigParInit();
        return false;
    }

    if(!file.open(QIODevice::ReadOnly)) {
        qDebug() << "mqtt config File open failed!";
        return false;
    }

    QJsonDocument jdc(QJsonDocument::fromJson(file.readAll()));
    rootObj = jdc.object();

    if(updateConfig == true)
    {
        QJsonObject subObj;
        QJsonArray subArray;
        QJsonArray subtopicArray;
        QJsonObject subscribeObj;
        QJsonArray pubtopicArray;
        QJsonObject pubObj;

        QString ConfigNUmStr = QString("Client_%1").arg(ActiveClientNum);

        /*重置参数*/
        MqttSubscribeTopic.clear();
        MqttSUBQos.clear();
        MqttPublishTopic.clear();
        MqttPUBQos.clear();

        /*判断是否是数组*/
        if(rootObj.contains("MQTT_CONFIG") && rootObj["MQTT_CONFIG"].isArray())
        {
            subArray = rootObj.value("MQTT_CONFIG").toArray();

            for(int i = 0;i < subArray.size(); i++)
            {
                /*跳过非激活客户端配置*/
                if(i != ActiveClientNum)
                {
                    continue;
                }

                /*转换为子对象*/
                subObj = subArray.at(i).toObject();

                if(subObj.contains("MqttConfigNum") && subObj["MqttConfigNum"].toString() == ConfigNUmStr)
                {
                    /*读取客户端ID*/
                    MqttClentID = ConfigNUmStr;

                    /*读取服务器域名地址*/
                    if(subObj.contains("MqttServerAddr") && subObj["MqttServerAddr"].isString())
                    {
                        MqttServerAddr = subObj["MqttServerAddr"].toString();
                    }

                    /*读取服务器IP地址*/
                    if(subObj.contains("MqttServerIp") && subObj["MqttServerIp"].isString())
                    {
                        MqttServerIp = subObj["MqttServerIp"].toString();
                    }

                    /*读取服务器端口号*/
                    if(subObj.contains("MqttServerPort") && subObj["MqttServerPort"].isDouble())
                    {
                        MqttServerPort = static_cast<quint16>(subObj["MqttServerPort"].toInt());
                    }

                    /*读取用户名*/
                    if(subObj.contains("MqttUserName") && subObj["MqttUserName"].isString())
                    {
                        MqttUserName = subObj["MqttUserName"].toString();
                    }

                    /*读取用户密码*/
                    if(subObj.contains("MqttUserKey") && subObj["MqttUserKey"].isString())
                    {
                        MqttUserKey = subObj["MqttUserKey"].toString();
                    }

                    /*读取保活时间*/
                    if(subObj.contains("MqttKeepAlive") && subObj["MqttKeepAlive"].isDouble())
                    {
                        MqttKeepAlive = static_cast<quint16>(subObj["MqttKeepAlive"].toInt());
                    }

                    /*读取订阅列表*/
                    if(subObj.contains("SubscribeTopic") && subObj["SubscribeTopic"].isArray())
                    {
                        subtopicArray = subObj.value("SubscribeTopic").toArray();
                        for(int index = 0;index < subtopicArray.size();index++)
                        {
                            /*转换为子对象*/
                            subscribeObj = subtopicArray.at(index).toObject();

                            if (subscribeObj.contains("MqttSubscribeTopic") && subscribeObj["MqttSubscribeTopic"].isString())
                            {
                               MqttSubscribeTopic.append(subscribeObj["MqttSubscribeTopic"].toString());
                            }
                            else
                            {
                                qDebug() << "未找到MqttSubscribeTopic";
                            }

                            /*读取订阅Qos*/
                            if (subscribeObj.contains("MqttSUBQos") && subscribeObj["MqttSUBQos"].isDouble())
                            {
                               MqttSUBQos.append(subscribeObj["MqttSUBQos"].toInt());
                            }
                            else
                            {
                                qDebug() << "未找到MqttSUBQos";
                            }
                        }
                    }

                    /*读取发布列表*/
                    if(subObj.contains("PublishTopic") && subObj["PublishTopic"].isArray())
                    {
                        pubtopicArray = subObj.value("PublishTopic").toArray();
                        for(int index = 0;index < pubtopicArray.size();index++)
                        {
                            /*转换为子对象*/
                            pubObj = pubtopicArray.at(index).toObject();
                            if (pubObj.contains("MqttPublishTopic") && pubObj["MqttPublishTopic"].isString())
                            {
                                MqttPublishTopic.append(pubObj["MqttPublishTopic"].toString());
                            }
                            else
                            {
                                qDebug() << "未找到MqttPublishTopic";
                            }

                            /*读取发布Qos*/
                            if (pubObj.contains("MqttPUBQos") && pubObj["MqttPUBQos"].isDouble())
                            {
                                MqttPUBQos.append(pubObj["MqttPUBQos"].toInt());
                            }
                            else
                            {
                                qDebug() << "未找到MqttPUBQos";
                            }
                        }
                    }
                }
            }//MqttConfigNum == ConfigNUmStr
            file.close();
            qDebug() << "ADDR-"<<MqttServerAddr;
            qDebug() << "IP-"<<MqttServerIp;
            qDebug() << "MqttServerPort-"<<MqttServerPort;
            qDebug() << "MqttUserName-"<<MqttUserName;
            qDebug() << "MqttUserKey-"<<MqttUserKey;
            qDebug() << "MqttKeepAlive-"<<MqttKeepAlive;
            for(int x = 0; x < MqttSubscribeTopic.size();x++)
            {
                qDebug() << "MqttSubscribeTopic"<<x<<"-"<<MqttSubscribeTopic.value(x);
            }
            for(int x = 0; x < MqttPublishTopic.size();x++)
            {
                qDebug() << "MqttPublishTopic"<<x<<"-"<<MqttPublishTopic.value(x);
            }

            for(int x = 0; x < MqttSUBQos.size();x++)
            {
                qDebug() << "MqttSUBQos"<<x<<"-"<<MqttSUBQos.value(x);
            }
            for(int x = 0; x < MqttPUBQos.size();x++)
            {
                qDebug() << "MqttPUBQos"<<x<<"-"<<MqttPUBQos.value(x);
            }
            return true;
        }
        else
        {
            qDebug() << "读取失败,重置mqtt配置";
            file.resize(0);
            file.close();
            Create_ConfigFile();
        }
    }
    else
    {
        return true;
    }
    return false;
}

/**
 * @brief Libmqttclient::ModifiedJson
 * @param configindex 客户端配置索引
 * @param id
 * @param value
 * @return 返回true则成功
 */
bool Libmqttclient::ModifiedJson(QJsonObject &rootObj ,quint8 configindex ,QString id ,bool value)
{
    bool ret = false;
    QJsonObject subObj;

    QJsonArray subArray;

    QJsonArray subscribeArray;  /**< 订阅发布公用数组*/
    QJsonArray pubArray;        /**< 发布数组*/
    QJsonObject sub_pubObj;     /**< 订阅发布公用对象*/

    /*判断是否是数组*/
    if(rootObj.contains("MQTT_CONFIG") && rootObj["MQTT_CONFIG"].isArray())
    {
        subArray = rootObj.value("MQTT_CONFIG").toArray();

        for(int i = 0; i< subArray.size(); i++)
        {
            /*转换为子对象*/
            subObj = subArray.at(i).toObject();

            /*修改指定数据*/
            if(i == configindex && id.isEmpty() == false)
            {
                if(subObj.contains(id) && subObj[id].isBool())
                {
                    ret = true;
                    subObj[id] = value;
                }
                else
                {
                    if(subObj.contains("SubscribeTopic") && subObj["SubscribeTopic"].isArray())
                    {
                        subscribeArray = subObj["SubscribeTopic"].toArray();
                        for(int index = 0;index < subscribeArray.size(); index++)
                        {
                            /*转换为子对象*/
                            sub_pubObj = subscribeArray.at(index).toObject();
                            if(sub_pubObj.contains(id) && sub_pubObj[id].isBool())
                            {
                                /*找到则更新*/
                                ret = true;
                                sub_pubObj[id] = value;
                                subscribeArray.replace(index ,sub_pubObj);
                            }
                        }
                        subObj.insert("SubscribeTopic" ,subscribeArray);
                    }
                    if(subObj.contains("PublishTopic") && subObj["PublishTopic"].isArray())
                    {
                        pubArray = subObj["PublishTopic"].toArray();
                        for(int index = 0;index < pubArray.size(); index++)
                        {
                            /*转换为子对象*/
                            sub_pubObj = pubArray.at(index).toObject();
                            if(sub_pubObj.contains(id) && sub_pubObj[id].isBool())
                            {
                                /*找到则更新*/
                                ret = true;
                                sub_pubObj[id] = value;
                                pubArray.replace(index ,sub_pubObj);
                            }
                        }
                        subObj.insert("PublishTopic" ,pubArray);
                    }
                }
                subArray.replace(i ,subObj);
                rootObj.insert("MQTT_CONFIG" ,subArray);
            }
        }
    }
    return ret;
}

/**
 * @brief Libmqttclient::ModifiedJson
 * @param rootObj
 * @param configindex
 * @param id
 * @param value
 * @param type
 * @return true 修改完成
 */
bool Libmqttclient::ModifiedJson(QJsonObject &rootObj , quint8 configindex , QString id , QString value)
{
    bool ret = false;
    QJsonObject subObj;

    QJsonArray subArray;

    QJsonArray subscribeArray;  /**< 订阅发布公用数组*/
    QJsonArray pubArray;        /**< 发布数组*/
    QJsonObject sub_pubObj;     /**< 订阅发布公用对象*/

    /*判断是否是数组*/
    if(rootObj.contains("MQTT_CONFIG") && rootObj["MQTT_CONFIG"].isArray())
    {
        subArray = rootObj.value("MQTT_CONFIG").toArray();

        for(int i = 0; i< subArray.size(); i++)
        {
            /*转换为子对象*/
            subObj = subArray.at(i).toObject();

            /*修改指定数据*/
            if(i == configindex && id.isEmpty() == false)
            {
                if(subObj.contains(id) && subObj[id].isString())
                {
                    ret = true;
                    subObj[id] = value;
                }
                else
                {
                    if(subObj.contains("SubscribeTopic") && subObj["SubscribeTopic"].isArray())
                    {
                        subscribeArray = subObj["SubscribeTopic"].toArray();
                        for(int index = 0;index < subscribeArray.size(); index++)
                        {
                            /*转换为子对象*/
                            sub_pubObj = subscribeArray.at(index).toObject();
                            if(sub_pubObj.contains(id) && sub_pubObj[id].isString())
                            {
                                /*找到则更新*/
                                ret = true;
                                sub_pubObj[id] = value;
                                subscribeArray.replace(index ,sub_pubObj);
                                break;
                            }
                        }
                        subObj.insert("SubscribeTopic" ,subscribeArray);
                    }
                    if(subObj.contains("PublishTopic") && subObj["PublishTopic"].isArray())
                    {
                        pubArray = subObj["PublishTopic"].toArray();
                        for(int index = 0;index < pubArray.size(); index++)
                        {
                            /*转换为子对象*/
                            sub_pubObj = pubArray.at(index).toObject();
                            if(sub_pubObj.contains(id) && sub_pubObj[id].isString())
                            {
                                /*找到则更新*/
                                ret = true;
                                sub_pubObj[id] = value;
                                pubArray.replace(index ,sub_pubObj);
                            }
                        }
                        subObj.insert("PublishTopic" ,pubArray);
                    }
                }
                subArray.replace(i ,subObj);
                rootObj["MQTT_CONFIG"] = subArray;
            }
        }
    }
    return ret;
}

/**
 * @brief Libmqttclient::ModifiedJson 逻辑问题不使用
 * @param rootObj
 * @param configindex
 * @param topic
 * @param value
 * @param type
 * @return true 修改完成
 */
bool Libmqttclient::ModifiedJson(QJsonObject &rootObj ,quint8 configindex ,QString id ,QString value ,QString topic)
{
    bool ret = false;
    QJsonObject subObj;

    QJsonArray subArray;

    QJsonArray subscribeArray;  /**< 订阅发布公用数组*/
    QJsonArray pubArray;        /**< 发布数组*/
    QJsonObject sub_pubObj;     /**< 订阅发布公用对象*/

    /*判断是否是数组*/
    if(rootObj.contains("MQTT_CONFIG") && rootObj["MQTT_CONFIG"].isArray())
    {
        subArray = rootObj.value("MQTT_CONFIG").toArray();

        for(int i = 0; i< subArray.size(); i++)
        {
            /*转换为子对象*/
            subObj = subArray.at(i).toObject();

            /*修改指定数据*/
            if(i == configindex && topic.isEmpty() == false)
            {
                if(subObj.contains(id) && subObj[id].isString())
                {
                    /*判断主题是否是需要修改的主题*/
                    if(subObj[id].toString() == topic)
                    {
                        ret = true;
                        subObj[id] = value;
                    }
                }
                else
                {
                    if(subObj.contains("SubscribeTopic") && subObj["SubscribeTopic"].isArray())
                    {
                        subscribeArray = subObj["SubscribeTopic"].toArray();
                        for(int index = 0;index < subscribeArray.size(); index++)
                        {
                            /*转换为子对象*/
                            sub_pubObj = subscribeArray.at(index).toObject();
                            if(sub_pubObj.contains(id) && sub_pubObj[id].isString())
                            {
                                /*判断主题是否是需要修改的主题*/
                                if(sub_pubObj[id].toString() == topic)
                                {
                                    /*找到则更新*/
                                    ret = true;
                                    sub_pubObj[id] = value;
                                    subscribeArray.replace(index ,sub_pubObj);
                                }
                            }
                        }
                        subObj.insert("SubscribeTopic" ,subscribeArray);
                    }
                    if(subObj.contains("PublishTopic") && subObj["PublishTopic"].isArray())
                    {
                        pubArray = subObj["PublishTopic"].toArray();
                        for(int index = 0;index < pubArray.size(); index++)
                        {
                            /*转换为子对象*/
                            sub_pubObj = pubArray.at(index).toObject();
                            if(sub_pubObj.contains(id) && sub_pubObj[id].isString())
                            {
                                /*判断主题是否是需要修改的主题*/
                                if(sub_pubObj[id].toString() == topic)
                                {
                                    /*找到则更新*/
                                    ret = true;
                                    sub_pubObj[id] = value;
                                    pubArray.replace(index ,sub_pubObj);
                                }
                            }
                        }
                        subObj.insert("PublishTopic" ,pubArray);
                    }
                }
                subArray.replace(i ,subObj);
                rootObj["MQTT_CONFIG"] = subArray;
            }
        }
    }
    return ret;
}

/**
 * @brief Libmqttclient::ModifiedJsonTopic
 * @param rootObj
 * @param configindex
 * @param id
 * @param type
 * @return true 修改完成
 */
bool Libmqttclient::ModifiedJsonTopic(QJsonObject &rootObj ,quint8 configindex ,QString id ,DATA_TYPE type)
{
    bool ret = false;

    QJsonArray subArray;
    QJsonObject subObj;
    QJsonArray subscribeArray;  /**< 订阅发布公用数组*/
    QJsonArray pubArray;        /**< 发布数组*/
    QJsonObject sub_Obj;        /**< 订阅发布公用对象*/
    QJsonObject pub_Obj;        /**< 订阅发布公用对象*/

    /*删除json中所有topic*/
    /*获取当前TOPIC列表,加入到json文件中*/

    /*判断是否是数组*/
    if(rootObj.contains("MQTT_CONFIG") && rootObj["MQTT_CONFIG"].isArray())
    {
        subArray = rootObj.value("MQTT_CONFIG").toArray();

        for(int i = 0; i< subArray.size(); i++)
        {
            /*转换为子对象*/
            subObj = subArray.at(i).toObject();

            if(i == configindex && id.isEmpty() == false)
            {
                if(subObj.contains("SubscribeTopic") && subObj["SubscribeTopic"].isArray())
                {
                    if(type == SUB_TOPIC)
                    {
                        while(subscribeArray.size())
                        {
                            subscribeArray.removeFirst();
                        }
                        for(int index = 0;index < ui->SUBSCRIBElistWidget->count();index++)
                        {
                            QListWidgetItem *item_sel = ui->SUBSCRIBElistWidget->item(index);
                            customerwidgetitem *cur_item = dynamic_cast<customerwidgetitem *>(ui->SUBSCRIBElistWidget->itemWidget(item_sel));
                            sub_Obj[id] = cur_item->text->text();
                            sub_Obj["MqttSUBQos"] = MqttSUBQos.value(index);
                            ret = true;
                            subscribeArray.append(sub_Obj);
                        }
                        /*禁止为空*/
                        if(subscribeArray.size() <= 0)
                        {
                            sub_Obj[id] = "";
                            sub_Obj["MqttSUBQos"] = 0;
                            subscribeArray.append(sub_Obj);
                        }
                        subObj["SubscribeTopic"] = subscribeArray;
                    }
                }

                if(subObj.contains("PublishTopic") && subObj["PublishTopic"].isArray())
                {
                    if(type == PUB_TOPIC)
                    {
                        while(pubArray.size())
                        {
                            pubArray.removeFirst();
                        }
                        for(int index = 0;index < ui->PUBLISHlistWidget->count();index++)
                        {
                            QListWidgetItem *item_sel = ui->PUBLISHlistWidget->item(index);
                            customerwidgetitem *cur_item = dynamic_cast<customerwidgetitem *>(ui->PUBLISHlistWidget->itemWidget(item_sel));
                            pub_Obj[id] = cur_item->text->text();
                            pub_Obj["MqttPUBQos"] = MqttPUBQos.value(index);
                            ret = true;
                            pubArray.append(pub_Obj);
                        }
                        /*禁止为空*/
                        if(pubArray.size() <= 0)
                        {
                            pub_Obj[id] = "";
                            pub_Obj["MqttPUBQos"] = 0;
                            pubArray.append(pub_Obj);
                        }
                        subObj["PublishTopic"] = pubArray;
                    }
                }

                subArray.replace(i ,subObj);
                rootObj["MQTT_CONFIG"] = subArray;
            }
        }
    }
    return ret;
}

/**
 * @brief Libmqttclient::ModifiedJsonRemove
 * @param rootObj
 * @param configindex
 * @param id
 * @param value
 * @param topic
 * @return true 移除完成
 */
bool Libmqttclient::ModifiedJsonRemoveTopic(QJsonObject &rootObj ,quint8 configindex ,QString id ,QString topic ,DATA_TYPE type)
{
    bool ret = false;
    QJsonObject subObj;

    QJsonArray subArray;

    QJsonArray subscribeArray;  /**< 订阅发布公用数组*/
    QJsonArray pubArray;        /**< 发布数组*/
    QJsonObject sub_pubObj;     /**< 订阅发布公用对象*/

    /*判断是否是数组*/
    if(rootObj.contains("MQTT_CONFIG") && rootObj["MQTT_CONFIG"].isArray())
    {
        subArray = rootObj.value("MQTT_CONFIG").toArray();

        for(int i = 0; i< subArray.size(); i++)
        {
            /*转换为子对象*/
            subObj = subArray.at(i).toObject();

            if(i == configindex && topic.isEmpty() == false)
            {
                if(subObj.contains("SubscribeTopic") && subObj["SubscribeTopic"].isArray())
                {
                    /*判断需移除的id*/
                    if(type == SUB_TOPIC)
                    {
                        subscribeArray = subObj["SubscribeTopic"].toArray();
                        for(int index = 0;index < subscribeArray.size(); index++)
                        {
                            /*转换为子对象*/
                            sub_pubObj = subscribeArray.at(index).toObject();
                            if(sub_pubObj.contains(id) && sub_pubObj[id].isString())
                            {
                                /*判断主题是否是需要删除的主题*/
                                if(sub_pubObj[id].toString() == topic)
                                {
                                    /*找到删除*/
                                    subscribeArray.removeAt(index);
                                    ret = true;
                                    break;
                                }
                            }
                        }
                        subObj.insert("SubscribeTopic" ,subscribeArray);
                    }
                }

                if(subObj.contains("PublishTopic") && subObj["PublishTopic"].isArray())
                {
                    /*判断需移除的id*/
                    if(type == PUB_TOPIC)
                    {
                        pubArray = subObj["PublishTopic"].toArray();
                        for(int index = 0;index < pubArray.size(); index++)
                        {
                            /*转换为子对象*/
                            sub_pubObj = pubArray.at(index).toObject();
                            if(sub_pubObj.contains(id) && sub_pubObj[id].isString())
                            {
                                /*判断主题是否是需要修改的主题*/
                                if(sub_pubObj[id].toString() == topic)
                                {
                                    /*找到则删除*/
                                    pubArray.removeAt(index);
                                    ret = true;
                                    break;
                                }
                            }
                        }
                        subObj.insert("PublishTopic" ,pubArray);
                    }
                }

                subArray.replace(i ,subObj);
                rootObj["MQTT_CONFIG"] = subArray;
            }
        }
    }
    return ret;
}

/**
 * @brief Libmqttclient::ModifiedJsonAddTopic
 * @param rootObj
 * @param configindex
 * @param id 订阅或者发布的json id
 * @param value 新增主题
 * @param qos qos json id
 * @param qosvalue 新增qos值
 * @param type
 * @return true 新增完成
 */
bool Libmqttclient::ModifiedJsonAddTopic(QJsonObject &rootObj ,quint8 configindex ,QString id ,QString value ,QString qos ,qint32 qosvalue ,DATA_TYPE type)
{
    bool ret = false;
    QJsonObject subObj;

    QJsonArray subArray;

    QJsonArray subscribeArray;  /**< 订阅发布公用数组*/
    QJsonArray pubArray;        /**< 发布数组*/
    QJsonObject sub_pubObj;     /**< 订阅发布公用对象*/

    /*判断是否是数组*/
    if(rootObj.contains("MQTT_CONFIG") && rootObj["MQTT_CONFIG"].isArray())
    {
        subArray = rootObj.value("MQTT_CONFIG").toArray();

        for(int i = 0; i< subArray.size(); i++)
        {
            /*转换为子对象*/
            subObj = subArray.at(i).toObject();

            /*增加指定主题数据*/
            if(i == configindex && id.isEmpty() == false)
            {
                if(subObj.contains("SubscribeTopic") && subObj["SubscribeTopic"].isArray())
                {
                    /*判断需增加的id*/
                    if(type == SUB_TOPIC)
                    {
                        subscribeArray = subObj["SubscribeTopic"].toArray();
                        for(int index = 0;index < subscribeArray.size(); index++)
                        {
                            /*转换为子对象*/
                            sub_pubObj = subscribeArray.at(index).toObject();
                            if(sub_pubObj.contains(id) && sub_pubObj[id].isString())
                            {
                                /*判断主题是否是需要修改的主题*/
                                if(sub_pubObj[id].toString() == value)
                                {
                                    /*找到重复直接退出*/
                                    ret = false;
                                    return ret;
                                }
                            }
                        }

                        /*新增*/
                        ret = true;
                        sub_pubObj[id] = value;
                        sub_pubObj[qos] = qosvalue;
                        subscribeArray.append(sub_pubObj);
                        subObj.insert("SubscribeTopic" ,subscribeArray);
                    }
                }

                if(subObj.contains("PublishTopic") && subObj["PublishTopic"].isArray())
                {
                    /*判断需增加的id*/
                    if(type == PUB_TOPIC)
                    {
                        pubArray = subObj["PublishTopic"].toArray();
                        for(int index = 0;index < pubArray.size(); index++)
                        {
                            /*转换为子对象*/
                            sub_pubObj = pubArray.at(index).toObject();
                            if(sub_pubObj.contains(id) && sub_pubObj[id].isString())
                            {
                                /*判断主题是否是需要修改的主题*/
                                if(sub_pubObj[id].toString() == value)
                                {
                                    /*找到重复直接退出*/
                                    ret = false;
                                    return ret;
                                }
                            }
                        }

                        /*新增*/
                        ret = true;
                        sub_pubObj[id] = value;
                        sub_pubObj[qos] = qosvalue;
                        pubArray.append(sub_pubObj);
                        subObj.insert("PublishTopic" ,pubArray);
                    }
                }

                subArray.replace(i ,subObj);
                rootObj["MQTT_CONFIG"] = subArray;
            }
        }
    }
    return ret;
}

/**
 * @brief Libmqttclient::ModifiedJson
 * @param rootObj
 * @param configindex
 * @param id
 * @param value
 * @return true 修改完成
 */
bool Libmqttclient::ModifiedJson(QJsonObject &rootObj ,quint8 configindex ,QString id ,qint32 value)
{
    bool ret = false;
    QJsonObject subObj;

    QJsonArray subArray;

    QJsonArray subscribeArray;  /**< 订阅发布公用数组*/
    QJsonArray pubArray;        /**< 发布数组*/
    QJsonObject sub_pubObj;     /**< 订阅发布公用对象*/

    /*判断是否是数组*/
    if(rootObj.contains("MQTT_CONFIG") && rootObj["MQTT_CONFIG"].isArray())
    {
        subArray = rootObj.value("MQTT_CONFIG").toArray();

        for(int i = 0; i< subArray.size(); i++)
        {
            /*转换为子对象*/
            subObj = subArray.at(i).toObject();

            /*修改指定数据*/
            if(i == configindex && id.isEmpty() == false)
            {
                if(subObj.contains(id) && subObj[id].isDouble())
                {
                    ret = true;
                    subObj[id] = value;
                }
                else
                {
                    if(subObj.contains("SubscribeTopic") && subObj["SubscribeTopic"].isArray())
                    {
                        subscribeArray = subObj["SubscribeTopic"].toArray();
                        for(int index = 0;index < subscribeArray.size(); index++)
                        {
                            /*转换为子对象*/
                            sub_pubObj = subscribeArray.at(index).toObject();
                            if(sub_pubObj.contains(id) && sub_pubObj[id].isDouble())
                            {
                                /*找到则更新*/
                                ret = true;
                                sub_pubObj[id] = value;
                                subscribeArray.replace(index ,sub_pubObj);
                                break;
                            }
                        }
                        subObj.insert("SubscribeTopic" ,subscribeArray);
                    }
                    if(subObj.contains("PublishTopic") && subObj["PublishTopic"].isArray())
                    {
                        pubArray = subObj["PublishTopic"].toArray();
                        for(int index = 0;index < pubArray.size(); index++)
                        {
                            /*转换为子对象*/
                            sub_pubObj = pubArray.at(index).toObject();
                            if(sub_pubObj.contains(id) && sub_pubObj[id].isDouble())
                            {
                                /*找到则更新*/
                                ret = true;
                                sub_pubObj[id] = value;
                                pubArray.replace(index ,sub_pubObj);
                                break;
                            }
                        }
                        subObj.insert("PublishTopic" ,pubArray);
                    }
                }
                subArray.replace(i ,subObj);
                rootObj.insert("MQTT_CONFIG" ,subArray);
            }
        }
    }
    return ret;
}

/**
 * @brief Libmqttclient::RemoveSubscribeToic
 * @param topic
 * @param Writeflash 是否写入flash
 * @return true 写入完成
 */
bool Libmqttclient::RemoveSubscribeToic(QString topic ,bool Writeflash ,quint8 configindex)
{
    bool ret = DeleteSubscribe(topic);
    if(Writeflash == true)
    {
        QJsonObject rootObj;
        QString id = "MqttSubscribeTopic";
        ModifiedJsonRemoveTopic(rootObj ,configindex ,id ,topic ,SUB_TOPIC);
        WriteJsonFile(rootObj);
        ReadConfigFromJson(rootObj);
    }
    return ret;
}

/**
 * @brief Libmqttclient::RemovePublishToic
 * @param topic
 * @param Writeflash 是否写入flash
 * @return true 写入完成
 */
bool Libmqttclient::RemovePublishToic(QString topic ,bool Writeflash ,quint8 configindex)
{
    bool ret = DeletePublish(topic);
    if(Writeflash == true)
    {
        QJsonObject rootObj;
        QString id = "MqttPublishTopic";
        ModifiedJsonRemoveTopic(rootObj ,configindex ,id ,topic ,PUB_TOPIC);
        WriteJsonFile(rootObj);
        ReadConfigFromJson(rootObj);
    }
    return ret;
}

/**
 * @brief Libmqttclient::AddSubscribeToic
 * @param topic
 * @param Writeflash 是否写入flash
 * @return true 写入完成
 */
bool Libmqttclient::AddSubscribeToic(QString topic ,qint32 QosValue ,bool Writeflash ,quint8 configindex)
{
    bool ret = AddSubscribe(topic);
    if(Writeflash == true)
    {
        QJsonObject rootObj;
        QString id = "MqttSubscribeTopic";
        QString qos = "MqttSUBQos";
        ModifiedJsonAddTopic(rootObj ,configindex ,id ,topic ,qos ,QosValue ,SUB_TOPIC);
        WriteJsonFile(rootObj);
        ReadConfigFromJson(rootObj);
    }
    return ret;
}

/**
 * @brief Libmqttclient::addPublishToic
 * @param topic
 * @param Writeflash 是否写入flash
 * @return true 写入完成
 */
bool Libmqttclient::AddPublishToic(QString topic ,qint32 QosValue ,bool Writeflash ,quint8 configindex)
{
    bool ret = AddPublish(topic);
    if(Writeflash == true)
    {
        QJsonObject rootObj;
        QString id = "MqttPublishTopic";
        QString qos = "MqttPUBQos";
        ModifiedJsonAddTopic(rootObj ,configindex ,id ,topic ,qos ,QosValue ,PUB_TOPIC);
        WriteJsonFile(rootObj);
        ReadConfigFromJson(rootObj);
    }
    return ret;
}

/**
 * @brief Libmqttclient::Modified_Config
 * @param ConfigrootObj
 */
void Libmqttclient::Modified_Config(QJsonObject &ConfigrootObj ,DATA_TYPE type)
{
    /*修改指定类型参数*/
    for(quint8 i = 0; i < MQTT_CLIENT_MAX_NUM; i++)
    {
        if(i == ActiveClientNum)
        {
            switch(type)
            {
                case HOST_NAME:
                    if(MqttServerAddr != ui->HOSTNAMElineEdit->text())
                    {
                        MqttServerAddr = ui->HOSTNAMElineEdit->text();

                        ModifiedJson(ConfigrootObj ,i ,QString("MqttServerAddr") ,MqttServerAddr);
                    }
                    break;
                case HOST_IP:
                    if(MqttServerIp != ui->HOSTIPlineEdit->text())
                    {
                        MqttServerIp = ui->HOSTIPlineEdit->text();

                        ModifiedJson(ConfigrootObj ,i ,QString("MqttServerIp") ,MqttServerIp);
                    }
                    break;
                case HOST_PORT:
                    {
                        if(MqttServerPort != ui->HOSTPORTlineEdit->text().toUShort())
                        {
                            MqttServerPort = ui->HOSTPORTlineEdit->text().toUShort();
                            qint32 port = MqttServerPort;
                            ModifiedJson(ConfigrootObj ,i ,QString("MqttServerPort") ,port);
                        }
                    }
                    break;
                case USER_NAME:
                    if(MqttUserName != ui->USERNAMElineEdit->text())
                    {
                        MqttUserName = ui->USERNAMElineEdit->text();

                        ModifiedJson(ConfigrootObj ,i ,QString("MqttUserName") ,MqttUserName);
                    }
                    break;
                case USER_KEY:
                    if(MqttUserKey != ui->KEYWORDlineEdit->text())
                    {
                        MqttUserKey = ui->KEYWORDlineEdit->text();

                        ModifiedJson(ConfigrootObj ,i ,QString("MqttUserKey") ,MqttUserKey);
                    }
                    break;
                case SUB_TOPIC:
                    ModifiedJsonTopic(ConfigrootObj ,i ,QString("MqttSubscribeTopic") ,SUB_TOPIC);
                    break;
                case PUB_TOPIC:
                    ModifiedJsonTopic(ConfigrootObj ,i ,QString("MqttPublishTopic") ,PUB_TOPIC);
                    break;
            }
        }
    }
}

/**
 * @brief Libmqttclient::WriteJsonFile
 * @param rootObj
 * @return
 */
bool Libmqttclient::WriteJsonFile(QJsonObject &rootObj)
{
    QFile file(MQTT_JSON_FILE);

    /*检测文件是否存在*/
    if(file.exists() == false)
    {
        Create_ConfigFile();
        return false;
    }

    /*清空操作*/
    if(!file.open(QIODevice::WriteOnly)) {
        qDebug() << "File open mqtt config failed!";
        return false;
    }
    file.resize(0);

    QJsonDocument jdc;
    jdc.setObject(rootObj);

    file.seek(0);/*从头开始写*/
    file.write(jdc.toJson());
    file.flush();
    file.close();
    return true;
}

/**
 * @brief Libmqttclient::ConfigFlagInit
 */
void Libmqttclient::ConfigFlagInit()
{
    DataChanged_Flag = 0;
}

void Libmqttclient::MqttConfigInit()
{
    MqttClient = new QMqttClient(this);
    /*连接信号*/
    connect(MqttClient ,&QMqttClient::connected ,this ,&Libmqttclient::slotConnected);
    /*断开信号*/
    connect(MqttClient ,&QMqttClient::disconnected ,this ,&Libmqttclient::slotDisconnected);
    /*ping响应信号*/
    connect(MqttClient ,&QMqttClient::pingResponseReceived ,this ,&Libmqttclient::slotPingResponse);
    /*新数据接收信号*/
    connect(MqttClient ,&QMqttClient::messageReceived ,this ,&Libmqttclient::slotMessageReceived);
    /*会话重连恢复信号*/
    connect(MqttClient ,&QMqttClient::brokerSessionRestored ,this ,&Libmqttclient::slotSessionRestored);
    /*认证请求信号*/
    connect(MqttClient ,&QMqttClient::authenticationRequested ,this ,&Libmqttclient::slotauthenticationRequested);
    /*认证完成信号*/
    connect(MqttClient ,&QMqttClient::authenticationFinished ,this ,&Libmqttclient::slotauthenticationFinished);

    /*打印协议版本信息*/
    QMqttClient::ProtocolVersion ver = MqttClient->QMqttClient::protocolVersion();
    switch(ver)
    {
        case QMqttClient::MQTT_3_1:
            ui->textBrowser->append("ProtocolVersion：V3.1");
            break;
        case QMqttClient::MQTT_3_1_1:
            ui->textBrowser->append("ProtocolVersion：V3.1.1");
            break;
        case QMqttClient::MQTT_5_0:
            ui->textBrowser->append("ProtocolVersion：V5.0");
            break;
    }

    /*获取默认加密配置*/
    sslConfig = QSslConfiguration::defaultConfiguration();

    /*设置自动保活*/
    MqttClient->setAutoKeepAlive(true);

    /*自动连接*/
    NetworkPingCheck(MqttServerIp);
}

/**
 * @brief Libmqttclient::UNsubscribe_all
 */
void Libmqttclient::UNsubscribe_all()
{
    while(ui->SUBSCRIBElistWidget->count())
    {
        QListWidgetItem *item_sel = ui->SUBSCRIBElistWidget->item(0);
        customerwidgetitem *cur_item = dynamic_cast<customerwidgetitem *>(ui->SUBSCRIBElistWidget->itemWidget(item_sel));
        MqttClient->unsubscribe(cur_item->text->text());
        ui->SUBSCRIBElistWidget->takeItem(0);
    }
    /*显示到消息区*/
    QString str;
    str = QDateTime::currentDateTime().toString();
    ui->textBrowser->append(str + green_prefix + ": unsubscribe all OK." + font_suffix);
    ui->textBrowser->moveCursor(QTextCursor::End);
}

/**
 * @brief Libmqttclient::UNpublish_all
 */
void Libmqttclient::UNpublish_all()
{
    while(ui->PUBLISHlistWidget->count())
    {
        ui->PUBLISHlistWidget->takeItem(0);
    }
    /*显示到消息区*/
    QString str;
    str = QDateTime::currentDateTime().toString();
    ui->textBrowser->append(str + green_prefix + ": unpublish all OK." + font_suffix);
    ui->textBrowser->moveCursor(QTextCursor::End);
}

/**
 * @brief Libmqttclient::MqttConfigRestore
 */
void Libmqttclient::MqttConfigRestore()
{
    /*将当前连接信息显示到编辑框中*/
    ui->HOSTNAMElineEdit->setText(MqttServerAddr);
    ui->HOSTIPlineEdit->setText(MqttServerIp);
    ui->HOSTPORTlineEdit->setText(QString("%1").arg(MqttServerPort));
    ui->USERNAMElineEdit->setText(MqttUserName);
    ui->KEYWORDlineEdit->setText(MqttUserKey);

    /*恢复订阅*/
    UNsubscribe_all();
    if(ui->SUBSCRIBElistWidget->count() < MqttSubscribeTopic.size())
    {
        for(qint32 i = 0; i < MqttSubscribeTopic.size();i++)
        {
            /*加入一条订阅*/
            AddSubscribe(MqttSubscribeTopic.value(i) ,false);
        }
    }

    /*恢复发布*/
    UNpublish_all();
    if(ui->PUBLISHlistWidget->count() < MqttPublishTopic.size())
    {
        for(qint32 i = 0; i < MqttPublishTopic.size();i++)
        {
            /*加入一条发布*/
            AddPublish(MqttPublishTopic.value(i) ,false);
        }
    }
}

/**
 * @brief Libmqttclient::AddSubscribe
 * @param topicStr
 * @param manual_flag
 * @return true 正确增加一条订阅
 */
bool Libmqttclient::AddSubscribe(QString topicStr ,bool manual_flag)
{
    if(topicStr.isEmpty() == true)
    {
        return false;
    }
    /*遍历查重*/
    for(qint32 i = 0;i < ui->SUBSCRIBElistWidget->count();i++)
    {
        QListWidgetItem *item_sel = ui->SUBSCRIBElistWidget->item(i);
        customerwidgetitem *cur_item =dynamic_cast<customerwidgetitem *>(ui->SUBSCRIBElistWidget->itemWidget(item_sel));

        if(cur_item->text->text() == topicStr)
        {
            if(manual_flag == true)
            {
                QMessageBox::critical(this,tr("Error"),tr("<p><span style='color: rgb(255, 0, 80);font-size: 24px;'>主题：%1已经订阅.</span></p>").arg(topicStr));
            }
            return false;
        }
    }

    /*订阅*/
    /*添加一条qos*/
    MqttSUBQos.append(ui->SUBSCRIBEQOSspinBox->value());
    auto subscription = MqttClient->subscribe(topicStr ,static_cast<quint8>(MqttSUBQos.last()&0x000000FF));
    if(!subscription)
    {
        /*处于手动订阅时显示错误消息对话框*/
        if(manual_flag == true)
        {
            if(MqttConnectState == false)
            {
                QMessageBox::critical(this,tr("Error"),tr("<p><span style='color: rgb(255, 0, 80);font-size: 24px;'>订阅失败！请查看是否连接?</span></p>"));
            }
            else if(ui->SUBSCRIBElineEdit->text().isEmpty() == true)
            {
                QMessageBox::critical(this,tr("Error"),tr("<p><span style='color: rgb(255, 0, 80);font-size: 24px;'>订阅失败！请填写订阅主题.</span></p>"));
            }
        }
        return false;
    }

    /*列表增加*/
    customerwidgetitem *Newitem = new customerwidgetitem;
    Newitem->text->setText(topicStr);
    Newitem->btn_del->setText("取消订阅");
    QListWidgetItem *NewListItem = new QListWidgetItem;
    NewListItem->setSizeHint(QSize(0 ,30));
    ui->SUBSCRIBElistWidget->addItem(NewListItem);
    ui->SUBSCRIBElistWidget->setItemWidget(NewListItem ,Newitem);

    /*连接取消订阅信号*/
    connect(Newitem->btn_del ,&QPushButton::clicked
            ,[this]()
            {
                int item_n = ui->SUBSCRIBElistWidget->currentRow();
                if(item_n < 0) return;
                /*取出控件，并转换*/
                QListWidgetItem *item_sel = ui->SUBSCRIBElistWidget->item(item_n);
                /*取消订阅*/
                customerwidgetitem *cur_item =dynamic_cast<customerwidgetitem *>(ui->SUBSCRIBElistWidget->itemWidget(item_sel));
                MqttClient->unsubscribe(cur_item->text->text());
                /*移除列表*/
                ui->SUBSCRIBElistWidget->takeItem(item_n);
            });
    return true;
}

/**
 * @brief Libmqttclient::DeleteSubscribe
 * @param topicStr
 * @param manual_flag 手动模式则显示报警弹窗
 * @return true 正确删除一条订阅
 */
bool Libmqttclient::DeleteSubscribe(QString topicStr ,bool manual_flag)
{
    if(topicStr.isEmpty() == false)
    {
        /*优先删除当前*/
        for(qint32 i = 0;i < ui->SUBSCRIBElistWidget->count();i++)
        {
            QListWidgetItem *item_sel = ui->SUBSCRIBElistWidget->item(i);
            customerwidgetitem *cur_item =dynamic_cast<customerwidgetitem *>(ui->SUBSCRIBElistWidget->itemWidget(item_sel));

            if(cur_item->text->text() == topicStr)
            {
                /*删除并取消订阅*/
                ui->SUBSCRIBElistWidget->takeItem(i);
                if(MqttSUBQos.size() > i)
                {
                    MqttSUBQos.takeAt(i);
                }
                MqttClient->unsubscribe(topicStr);
                return true;
            }
        }
    }

    /*删除列表*/
    if(ui->SUBSCRIBElistWidget->count() > 0)
    {
        QListWidgetItem *item_sel = ui->SUBSCRIBElistWidget->item(ui->SUBSCRIBElistWidget->count()-1);
        customerwidgetitem *cur_item =dynamic_cast<customerwidgetitem *>(ui->SUBSCRIBElistWidget->itemWidget(item_sel));
        MqttClient->unsubscribe(cur_item->text->text());
        ui->SUBSCRIBElistWidget->takeItem(ui->SUBSCRIBElistWidget->count()-1);
        if(MqttSUBQos.size() > 0)
        {
            MqttSUBQos.takeAt(MqttSUBQos.size()-1);
        }
        return true;
    }
    /*提示信息*/
    if(manual_flag == true)
    {
        ui->SUBSCRIBElineEdit->setText("");
        QMessageBox::critical(this,tr("Error"),tr("<p><span style='color: rgb(255, 0, 80);font-size: 24px;'>取消订阅失败！未找到.</span></p>"));
    }
    return false;
}

/**
 * @brief Libmqttclient::AddPublish
 * @param topicStr
 * @param manual_flag
 * @return true 正确新增一条允许发布的主题
 */
bool Libmqttclient::AddPublish(QString topicStr ,bool manual_flag)
{
    if(topicStr.isEmpty() == true)
    {
        return false;
    }
    /*遍历查重*/
    for(qint32 i = 0;i < ui->PUBLISHlistWidget->count();i++)
    {
        QListWidgetItem *item_sel = ui->PUBLISHlistWidget->item(i);
        customerwidgetitem *cur_item =dynamic_cast<customerwidgetitem *>(ui->PUBLISHlistWidget->itemWidget(item_sel));

        if(cur_item->text->text() == topicStr)
        {
            if(manual_flag == true)
            {
                QMessageBox::critical(this,tr("Error"),tr("<p><span style='color: rgb(255, 0, 80);font-size: 24px;'>主题：%1已经发布.</span></p>").arg(topicStr));
            }
            return false;
        }
    }

    /*发布*/
    /*添加一条qos*/
    MqttPUBQos.append(ui->PUBLISHQOSspinBox->value());
    qint32 ret = MqttClient->publish(topicStr ,QByteArray() ,static_cast<quint8>(MqttPUBQos.last()&0x000000FF));
    if(ret != 0)
    {
        qDebug() << "无法发布！,可能可能服务等级错误";
        if(manual_flag == true)
        {
            if(MqttConnectState == false)
            {
                QMessageBox::critical(this,tr("Error"),tr("<p><span style='color: rgb(255, 0, 80);font-size: 24px;'>发布失败！请查看是否连接?</span></p>"));
            }
            else if(ui->PUBLISHlineEdit->text().isEmpty() == true)
            {
                QMessageBox::critical(this,tr("Error"),tr("<p><span style='color: rgb(255, 0, 80);font-size: 24px;'>发布失败！请填写发布主题.</span></p>"));
            }
        }
        return false;
    }

    /*列表增加*/
    customerwidgetitem *Newitem = new customerwidgetitem;
    Newitem->text->setText(topicStr);
    Newitem->btn_del->setText("取消发布");
    QListWidgetItem *NewListItem = new QListWidgetItem;
    NewListItem->setSizeHint(QSize(0 ,30));
    ui->PUBLISHlistWidget->addItem(NewListItem);
    ui->PUBLISHlistWidget->setItemWidget(NewListItem ,Newitem);

    /*连接取消发布信号*/
    connect(Newitem->btn_del ,&QPushButton::clicked
            ,[this]()
            {
                int item_n = ui->PUBLISHlistWidget->currentRow();
                if(item_n < 0) return;
                /*取消发布*/
                /*移除列表*/
                ui->PUBLISHlistWidget->takeItem(item_n);
            });
    return true;
}

/**
 * @brief Libmqttclient::DeletePublish
 * @param topicStr
 * @param manual_flag
 * @return  true 正确禁止一条发布的主题
 */
bool Libmqttclient::DeletePublish(QString topicStr ,bool manual_flag)
{
    if(topicStr.isEmpty() == false)
    {
        /*优先删除当前*/
        for(qint32 i = 0;i < ui->PUBLISHlistWidget->count();i++)
        {
            QListWidgetItem *item_sel = ui->PUBLISHlistWidget->item(i);
            customerwidgetitem *cur_item =dynamic_cast<customerwidgetitem *>(ui->PUBLISHlistWidget->itemWidget(item_sel));

            if(cur_item->text->text() == topicStr)
            {
                /*删除*/
                ui->PUBLISHlistWidget->takeItem(i);
                if(MqttPUBQos.size() > i)
                {
                    MqttPUBQos.takeAt(i);
                }
                return true;
            }
        }
    }
    /*删除列表*/
    if(ui->PUBLISHlistWidget->count() > 0)
    {
        ui->PUBLISHlistWidget->takeItem(ui->PUBLISHlistWidget->count()-1);
        if(MqttPUBQos.size() > 0)
        {
            MqttPUBQos.takeAt(MqttPUBQos.size()-1);
        }
        return true;
    }
    if(manual_flag == true)
    {
        ui->PUBLISHlineEdit->setText("");
        QMessageBox::critical(this,tr("Error"),tr("<p><span style='color: rgb(255, 0, 80);font-size: 24px;'>取消发布失败！未找到.</span></p>"));
    }
    return false;
}

/**
 * @brief Libmqttclient::PublishMsg
 * @param topicStr
 * @param msg
 * @return
 */
qint32 Libmqttclient::PublishMsg(QString topicStr ,QByteArray msg)
{
    qint32 ret = -1;
    QString str = QDateTime::currentDateTime().toString();
    if(DebugMode == false)
    {
        /*遍历确认发布主题存在*/
        for(qint32 i = 0;i < ui->PUBLISHlistWidget->count();i++)
        {
            QListWidgetItem *item_sel = ui->PUBLISHlistWidget->item(i);
            customerwidgetitem *cur_item =dynamic_cast<customerwidgetitem *>(ui->PUBLISHlistWidget->itemWidget(item_sel));

            if(cur_item->text->text() == topicStr)
            {
                /*发布消息*/
                ret = MqttClient->publish(topicStr ,msg ,static_cast<quint8>(MqttPUBQos.value(i)&0x000000FF));
                break;
            }
        }
        CheckDebugbrowserIsFull();
        if(ret != 0)
        {
            /*显示到消息区*/
            ui->textBrowser->append(str + ": Send " + green_prefix +"【" + msg + "】" + font_suffix);
        }
        else
        {
            ui->textBrowser->append(str + red_prefix + ": Send msg Error." + font_suffix);
        }
    }
    ui->textBrowser->moveCursor(QTextCursor::End);
    return ret;
}

/**
 * @brief Libmqttclient::SetReconnectInterval
 * @param msec
 */
void Libmqttclient::SetReconnectInterval(qint32 msec)
{
    ReconnectTimer->setInterval(msec);
}

/**
 * @brief maqtt::NetworkConnectCheck
 * @param netaddr
 * @return true 外网访问正常
 */
bool Libmqttclient::NetworkConnectCheck(QString netaddr)
{
    QHostInfo info = QHostInfo::fromName(netaddr);
    if(info.error() != QHostInfo::NoError)
    {
        qDebug() << "Lookup failed:" << info.errorString();
        return false;
    }
    return true;
}

/**
 * @brief Libmqttclient::NetworkPingCheck
 * @param netaddr
 * @return true 可以ping通
 */
void Libmqttclient::NetworkPingCheck(QString netaddr)
{
#if defined(Q_CC_MINGW)
    QString cmdstr="ping "+netaddr;
#else
    QString cmdstr="ping "+netaddr + " -c4";
#endif

    Exec.start(cmdstr);

    /*异步非阻塞*/
    NetworkCheckTimer->start();

    /*阻塞等待ping完成*/
//    exc.waitForFinished(-1);
//    /*获取ping结果*/
//    QString Retstr = exc.readAll();
}

/**
 * @brief Libmqttclient::SwitchClientConfig
 * @param clientNum
 */
void Libmqttclient::SwitchClientConfig(quint8 clientNum)
{
    quint8 temp = ActiveClientNum;
    if(clientNum != temp)
    {
        QJsonObject rootObj;
        temp = clientNum;
        temp = temp>=MQTT_CLIENT_MAX_NUM?MQTT_CLIENT_MAX_NUM-1:temp;
        ActiveClientNum = temp;
        /*将新配置读取到内存*/
        ReadConfigFromJson(rootObj);

        /*更新网络参数标题*/
        ui->NETWOEKgroupBox->setTitle(QString("网络%1连接参数").arg(ActiveClientNum));

        /*断开连接*/
        if(MqttConnectState == true)
        {
            DisconnectToMqttServer();
        }
        /*自动连接*/
        NetworkPingCheck(MqttServerIp);
    }
}

/**
 * @brief Libmqttclient::setUsername
 * @param user
 * @param Key
 * @param Writeflash
 * @param configindex
 */
void Libmqttclient::setUsername(QString &user ,QString &Key ,bool Writeflash ,quint8 configindex)
{
    quint8 temp = ActiveClientNum;
    ui->USERNAMElineEdit->setText(user);
    ui->KEYWORDlineEdit->setText(Key);
    if(Writeflash == true)
    {
        ActiveClientNum = configindex;
        DataChanged_Flag |= USER_NAME_CHANGED;
        on_confirmpushButton_clicked();
        ActiveClientNum = temp;
    }
    else
    {
        MqttUserName = user;
        MqttUserKey = Key;
    }
}

/**
 * @brief Libmqttclient::setHostIPAddr
 * @param addr
 * @param Writeflash
 * @param configindex
 */
void Libmqttclient::setHostIPAddr(QString &addr ,bool Writeflash ,quint8 configindex)
{
    quint8 temp = ActiveClientNum;
    ui->HOSTIPlineEdit->setText(addr);

    if(Writeflash == true)
    {
        ActiveClientNum = configindex;
        DataChanged_Flag |= SERVER_IP_CHANGED;
        on_confirmpushButton_clicked();
        ActiveClientNum = temp;
    }
    else
    {
        MqttServerIp = addr;
    }
}

/**
 * @brief Libmqttclient::setHostRealmAddr
 * @param addr
 * @param Writeflash
 * @param configindex
 */
void Libmqttclient::setHostRealmAddr(QString &addr ,bool Writeflash ,quint8 configindex)
{
    quint8 temp = ActiveClientNum;
    ui->HOSTNAMElineEdit->setText(addr);

    if(Writeflash == true)
    {
        ActiveClientNum = configindex;
        DataChanged_Flag |= HOST_NAME_CHANGED;
        on_confirmpushButton_clicked();
        ActiveClientNum = temp;
    }
    else
    {
        MqttServerAddr = addr;
    }
}

/**
 * @brief Libmqttclient::setHostPort
 * @param port
 * @param Writeflash
 * @param configindex
 */
void Libmqttclient::setHostPort(quint16 port ,bool Writeflash ,quint8 configindex)
{
    quint8 temp = ActiveClientNum;
    ui->HOSTPORTlineEdit->setText(QString("%1").arg(port));

    if(Writeflash == true)
    {
        ActiveClientNum = configindex;
        DataChanged_Flag |= SERVER_PORT_CHANGED;
        on_confirmpushButton_clicked();
        ActiveClientNum = temp;
    }
    else
    {
        MqttServerPort = port;
    }
}

/**
 * @brief Libmqttclient::setProtocolVersion
 * @param protocolVersion
 */
void Libmqttclient::setProtocolVersion(QMqttClient::ProtocolVersion protocolVersion)
{
    MqttClient->setProtocolVersion(protocolVersion);
}

/**
 * @brief Libmqttclient::setWillConfig
 * @param willQoS 设置遗属qos
 * @param willTopic 设置遗属主题
 * @param willMessage 设置遗属消息
 * @param willRetain 设置遗属保留
 */
void Libmqttclient::setWillMsgConfig(const QString &willTopic ,const QByteArray &willMessage ,quint8 willQoS ,bool willRetain)
{
    MqttClient->setWillQoS(willQoS);
    MqttClient->setWillTopic(willTopic);
    MqttClient->setWillMessage(willMessage);
    MqttClient->setWillRetain(willRetain);
}

/**
 * @brief Libmqttclient::setCleanSession
 * @param cleanSession flase 建立持久连接
 */
void Libmqttclient::setCleanSession(bool cleanSession)
{
     MqttClient->setCleanSession(cleanSession);
}

/**
 * @brief Libmqttclient::GetMqttConnectState
 * @return ClientState 0未连接 1连接中 2已连接
 */
QMqttClient::ClientState Libmqttclient::GetMqttConnectState()const
{
    return MqttClient->state();
}

/**
 * @brief Libmqttclient::ConnectToMqttServer
 * @param useEditbox 是否使用编辑框内参数，默认是
 * @return true 连接操作完成
 */
bool Libmqttclient::ConnectToMqttServer(bool useEditbox)
{
    /*防止空连接，造成死等*/
    if(MqttServerIp.isEmpty() == true)
    {
        return false;
    }

    if(useEditbox == true)
    {
        /*读取客户端id*/
        if(ui->NETWOEKgroupBox->title().isEmpty() == false)
            MqttClentID = ui->NETWOEKgroupBox->title();

        /*读取域名*/
        if(ui->HOSTNAMElineEdit->text().isEmpty() == false)
            MqttServerIp = ui->HOSTNAMElineEdit->text();

        /*读取服务器IP地址*/
        if(ui->HOSTIPlineEdit->text().isEmpty() == false)
            MqttServerIp = ui->HOSTIPlineEdit->text();

        /*读取端口号*/
        MqttServerPort = ui->HOSTPORTlineEdit->text().toUShort();

        /*读取用户名及密码*/
        MqttUserName = ui->USERNAMElineEdit->text();
        MqttUserKey = ui->KEYWORDlineEdit->text();
    }

    /*连接设置*/
    MqttClient->setClientId(MqttClentID);
    MqttClient->setHostname(MqttServerIp);
    MqttClient->setPort(MqttServerPort);
    MqttClient->setUsername(MqttUserName);
    MqttClient->setPassword(MqttUserKey);
#if defined(Q_CC_MINGW)
    if(UseSSL == true)
    {
        MqttClient->connectToHostEncrypted(sslConfig);
    }
    else
    {
        MqttClient->connectToHost();
    }
#else
    MqttClient->connectToHost();
#endif
    /*设置为连接中状态,不可再次点击连接*/
    ui->CONNECTpushButton->setText("连接中");
    ui->CONNECTSTATElabel->setText("");
    ui->CONNECTSTATElabel->setStyleSheet(m_yellow_SheetStyle);
    ui->CONNECTpushButton->setEnabled(false);
    ui->HOSTNAMElineEdit->setEnabled(false);
    ui->HOSTIPlineEdit->setEnabled(false);
    ui->HOSTIPlineEdit->setEnabled(false);
    ui->HOSTPORTlineEdit->setEnabled(false);
    ui->USERNAMElineEdit->setEnabled(false);
    ui->KEYWORDlineEdit->setEnabled(false);
    return true;
}

/**
 * @brief Libmqttclient::DisconnectToMqttServer
 * @return true 断连操作完成
 */
bool Libmqttclient::DisconnectToMqttServer()
{
    MqttClient->disconnectFromHost();
    /*设置为断开状态*/
    MqttConnectState = false;
    ui->CONNECTpushButton->setText("启动连接");
    ui->CONNECTSTATElabel->setStyleSheet(m_red_SheetStyle);
    /*允许输入*/
    ui->CONNECTpushButton->setEnabled(true);
    ui->HOSTNAMElineEdit->setEnabled(true);
    ui->HOSTIPlineEdit->setEnabled(true);
    ui->HOSTIPlineEdit->setEnabled(true);
    ui->HOSTPORTlineEdit->setEnabled(true);
    ui->USERNAMElineEdit->setEnabled(true);
    ui->KEYWORDlineEdit->setEnabled(true);
    return true;
}

/**
 * @brief Libmqttclient::slotConnected
 */
void Libmqttclient::slotConnected()
{
    qDebug() << "mqtt连接成功，停止重连检测定时器";
    ReconnectTimer->stop();
    ui->CONNECTSTATElabel->setStyleSheet(m_green_SheetStyle);
    /*连接成功，允许断开连接*/
    ui->CONNECTpushButton->setText("断开连接");
    ui->CONNECTpushButton->setEnabled(true);
    MqttConnectState = true;

    /*mqtt参数恢复显示区*/
    MqttConfigRestore();

    /*显示到消息区*/
    QString str;
    CheckDebugbrowserIsFull();
    str = QDateTime::currentDateTime().toString();
    ui->textBrowser->append(str + green_prefix + ": Connected To Server OK." + font_suffix);
    ui->textBrowser->moveCursor(QTextCursor::End);
}

/**
 * @brief Libmqttclient::slotDisconnected
 */
void Libmqttclient::slotDisconnected()
{
    qDebug() << "mqtt连接已断开,检测是否重新连接";
    ReconnectTimer->start();
    ui->CONNECTSTATElabel->setStyleSheet(m_red_SheetStyle);
    /*断开成功，允许连接*/
    ui->CONNECTpushButton->setText("启动连接");
    /*允许输入*/
    ui->CONNECTpushButton->setEnabled(true);
    ui->HOSTNAMElineEdit->setEnabled(true);
    ui->HOSTIPlineEdit->setEnabled(true);
    ui->HOSTIPlineEdit->setEnabled(true);
    ui->HOSTPORTlineEdit->setEnabled(true);
    ui->USERNAMElineEdit->setEnabled(true);
    ui->KEYWORDlineEdit->setEnabled(true);
    MqttConnectState = false;

    /*显示到消息区*/
    QString str;
    str = QDateTime::currentDateTime().toString();
    CheckDebugbrowserIsFull();
    ui->textBrowser->append(str + red_prefix + ": Disconnected From Server OK." + font_suffix);
    ui->textBrowser->moveCursor(QTextCursor::End);
}

/**
 * @brief Libmqttclient::slotPingResponse
 */
void Libmqttclient::slotPingResponse()
{
    static quint8 CurrentlineCnt = 0;
    QString str;
    if(DebugMode == false)
    {
        CheckDebugbrowserIsFull();
        str = QDateTime::currentDateTime().toString();
        ui->textBrowser->append(str + blue_prefix + ": Ping Request Return OK." + font_suffix);
        ui->textBrowser->moveCursor(QTextCursor::End);
    }
    CurrentlineCnt++;
    if(CurrentlineCnt > 20)
    {
        CurrentlineCnt = 0;
        ui->textBrowser->clearHistory();
        ui->textBrowser->clear();
        ui->textBrowser->setText("");
    }
}

/**
 * @brief Libmqttclient::slotMessageReceived
 * @param message
 * @param topic
 */
void Libmqttclient::slotMessageReceived(const QByteArray &message, const QMqttTopicName &topic)
{
    QString str = white_prefix + QDateTime::currentDateTime().toString();

    str += QLatin1String(": Received Topic: ") + red_prefix + topic.name() + font_suffix;

    str += QLatin1String(" Message: ") + font_suffix;// + QLatin1Char('\n');

    /*显示到消息区*/
    CheckDebugbrowserIsFull();
    ui->textBrowser->append(str);
    ui->textBrowser->append(green_prefix +"【" + message + "】" + font_suffix);
    ui->textBrowser->moveCursor(QTextCursor::End);

    emit MessageReceived(message ,topic);
}

/**
 * @brief Libmqttclient::slotSessionRestored
 */
void Libmqttclient::slotSessionRestored()
{
    ui->CONNECTSTATElabel->setStyleSheet(m_green_SheetStyle);
}

/**
 * @brief Libmqttclient::DealTimeout
 */
void Libmqttclient::DealTimeout()
{
    /*检测返回的结果，最长25s*/
    qDebug() << "自启连定时器运行中";

#if defined(Q_CC_MINGW)
    QString key = "往返行程的估计时间";
#else
    QString key = "4 received, 0% packet loss";
#endif
    /*返回不等于-1，说明ping结果包含"往返行程的估计时间"字段，则说明ping成功，网络可达;等于-1，表示没有此字段，说明ping不通*/
    if((-1 != PINGRetstr.indexOf(key)))
    {
       if(MqttConnectState == false)
       {
           DisconnectToMqttServer();
           qDebug() << "连接mqtt服务器中";
           ConnectToMqttServer(false);
       }
       qDebug() << "停止自启连定时器";
       NetworkCheckTimer->stop();
       ConnectErrorCnt = 0;
       PINGRetstr.clear();
    }
    else
    {
       qDebug() << PINGRetstr;
       ConnectErrorCnt++;
       if(ConnectErrorCnt > 4)
       {
           qDebug() << "mqtt连接失败";
           emit NetworkPingError();
           NetworkCheckTimer->stop();
           ConnectErrorCnt = 0;
           PINGRetstr.clear();
       }
    }
}

/**
 * @brief Libmqttclient::DealPageTimeout
 */
void Libmqttclient::DealDisconnectTimeout()
{
    qDebug() << "重连检测定时器运行中";
    if(AUTO_Connect_Active == true && MqttConnectState == false)
    {
        /*自动连接*/
        qDebug() << "重连...";
        ConnectToMqttServer(true);
    }
}

/**
 * @brief Libmqttclient::slotReadStandardOutput
 */
void Libmqttclient::slotexeFinished()
{
    /*将ping返回结果累加*/
#if defined(Q_CC_MINGW)
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    PINGRetstr += codec->toUnicode(Exec.readAll());
#else
    QTextCodec *codec = QTextCodec::codecForName("UTF8");
    PINGRetstr += Exec.readAll();
#endif
}

/**
 * @brief Libmqttclient::slotauthenticationRequested
 * @param p
 */
void Libmqttclient::slotauthenticationRequested(const QMqttAuthenticationProperties &p)
{
    Q_UNUSED(p)
    ui->textBrowser->append(white_prefix + "--->authenticationRequested." + font_suffix);
}

/**
 * @brief Libmqttclient::slotauthenticationFinished
 * @param p
 */
void Libmqttclient::slotauthenticationFinished(const QMqttAuthenticationProperties &p)
{
    Q_UNUSED(p)
    ui->textBrowser->append(green_prefix + "<---authenticationFinished." + font_suffix);
}

/**
 * @brief lookedUp
 * @param host
 */
void Libmqttclient::lookedUp(const QHostInfo &host)
{
    if(host.error() != QHostInfo::NoError)
    {
        qDebug() << "Lookup failed:" << host.errorString();
    }
    else
    {
        MqttServerIp = host.addresses().first().toString();
        /*连接mqtt服务器*/
        ConnectToMqttServer();
    }
}

/**
 * @brief Libmqttclient::on_CONNECTpushButton_clicked
 */
void Libmqttclient::on_CONNECTpushButton_clicked()
{
    /*判断当前按钮功能*/
    if(ui->CONNECTpushButton->text() == "启动连接")
    {
        if(ui->HOSTPORTlineEdit->text().isEmpty() || (ui->HOSTIPlineEdit->text().isEmpty() && ui->HOSTNAMElineEdit->text().isEmpty()))
        {
            QMessageBox::critical(this,tr("Error"),tr("<p><span style='color: rgb(255, 0, 80);font-size: 24px;'>请检查参数！.</span></p>"));
            return;
        }

        /*优先Ip输入栏作为服务器设置*/
        if(ui->HOSTIPlineEdit->text().isEmpty() == false)
        {
            MqttServerIp = ui->HOSTIPlineEdit->text();
        }
        else
        {
            /*转换域名为ip*/
            MqttServerAddr = ui->HOSTNAMElineEdit->text();
            /*检测网络连接状态*/
            bool ret = NetworkConnectCheck(QString("baidu.com"));
            if(ret == false)
            {
                return;
            }
            QHostInfo::lookupHost(MqttServerAddr ,this ,SLOT(lookedUp(QHostInfo)));
            return;
        }

        /*开启自动重连*/
        AUTO_Connect_Active = true;

        /*连接mqtt服务器*/
        ConnectToMqttServer();
    }
    else if(ui->CONNECTpushButton->text() == "断开连接")
    {
        AUTO_Connect_Active = false;/**< 手动断开禁止自动重连*/
        DisconnectToMqttServer();
    }
}

/**
 * @brief Libmqttclient::on_SUBSCRIBEpushButton_clicked
 */
void Libmqttclient::on_SUBSCRIBEpushButton_clicked()
{
    /*获取需订阅的主题*/
    QString topicStr = ui->SUBSCRIBElineEdit->text();
    /*增加一条订阅*/
    AddSubscribe(topicStr ,true);
    DataChanged_Flag |= SUBSCRIBE_CHANGED;
}

/**
 * @brief Libmqttclient::on_UNSUBSCRIBEpushButton_clicked
 */
void Libmqttclient::on_UNSUBSCRIBEpushButton_clicked()
{
    /*取消当前订阅*/
    QString topicStr = ui->SUBSCRIBElineEdit->text();
    /*删除一条订阅*/
    DeleteSubscribe(topicStr ,true);
    DataChanged_Flag |= SUBSCRIBE_CHANGED;
}

/**
 * @brief Libmqttclient::on_SUBSCRIBEQOSspinBox_valueChanged
 * @param arg1
 */
void Libmqttclient::on_SUBSCRIBEQOSspinBox_valueChanged(int arg1)
{
    Q_UNUSED(arg1)
    DataChanged_Flag |= SUBSCRIBE_QOS_CHANGED;
}

/**
 * @brief Libmqttclient::on_PUBLISHpushButton_clicked 注册功能，未注册发布的主题将禁止发布
 */
void Libmqttclient::on_PUBLISHpushButton_clicked()
{
    /*获取需发布的主题*/
    QString topicStr = ui->PUBLISHlineEdit->text();
    /*增加一条发布*/
    AddPublish(topicStr ,true);
    DataChanged_Flag |= PUBLISH_CHANGED;
}

/**
 * @brief Libmqttclient::on_UNPUBLISHpushButton_clicked
 */
void Libmqttclient::on_UNPUBLISHpushButton_clicked()
{
    /*取消发布*/
    QString topicStr = ui->PUBLISHlineEdit->text();
    /*删除一条发布*/
    DeletePublish(topicStr ,true);
    DataChanged_Flag |= PUBLISH_CHANGED;
}

/**
 * @brief Libmqttclient::on_PUBLISHQOSspinBox_valueChanged
 * @param arg1
 */
void Libmqttclient::on_PUBLISHQOSspinBox_valueChanged(int arg1)
{
    Q_UNUSED(arg1)
    DataChanged_Flag |= PUBLISH_QOS_CHANGED;
}

/**
 * @brief Libmqttclient::on_returnpushButton_clicked
 */
void Libmqttclient::on_returnpushButton_clicked()
{
    DataChanged_Flag = 0;
    this->hide();
    emit MqttwindowIsClose();
}

/**
 * @brief Libmqttclient::on_confirmpushButton_clicked
 */
void Libmqttclient::on_confirmpushButton_clicked()
{
    bool ConfigActive = false;

    /*JSON配置读取*/
    QJsonObject ConfigrootObj;
    if(DataChanged_Flag != 0)
    {
        ConfigActive = true;
        ReadConfigFromJson(ConfigrootObj ,false);
    }

    if(DataChanged_Flag & HOST_NAME_CHANGED)
    {
        Modified_Config(ConfigrootObj ,HOST_NAME);
        DataChanged_Flag &= ~HOST_NAME_CHANGED;
    }
    if(DataChanged_Flag & SERVER_IP_CHANGED)
    {
        Modified_Config(ConfigrootObj ,HOST_IP);
        DataChanged_Flag &= ~SERVER_IP_CHANGED;
    }
    if(DataChanged_Flag & SERVER_PORT_CHANGED)
    {
        Modified_Config(ConfigrootObj ,HOST_PORT);
        DataChanged_Flag &= ~SERVER_PORT_CHANGED;
    }
    if(DataChanged_Flag & USER_NAME_CHANGED)
    {
        Modified_Config(ConfigrootObj ,USER_NAME);
        DataChanged_Flag &= ~USER_NAME_CHANGED;
    }
    if(DataChanged_Flag & USER_KEY_CHANGED)
    {
        Modified_Config(ConfigrootObj ,USER_KEY);
        DataChanged_Flag &= ~USER_KEY_CHANGED;
    }
    if(DataChanged_Flag & SUBSCRIBE_CHANGED)
    {
        Modified_Config(ConfigrootObj ,SUB_TOPIC);
        DataChanged_Flag &= ~SUBSCRIBE_CHANGED;
    }
    if(DataChanged_Flag & SUBSCRIBE_QOS_CHANGED)
    {

        DataChanged_Flag &= ~SUBSCRIBE_QOS_CHANGED;
    }
    if(DataChanged_Flag & PUBLISH_CHANGED)
    {
        Modified_Config(ConfigrootObj ,PUB_TOPIC);
        DataChanged_Flag &= ~PUBLISH_CHANGED;
    }
    if(DataChanged_Flag & PUBLISH_QOS_CHANGED)
    {

        DataChanged_Flag &= ~PUBLISH_QOS_CHANGED;
    }

    /*JSON配置写入，再次读取更新到flash*/
    if(ConfigActive)
    {
        WriteJsonFile(ConfigrootObj);
        ReadConfigFromJson(ConfigrootObj);
    }
}

/**
 * @brief Libmqttclient::on_HOSTNAMElineEdit_editingFinished
 */
void Libmqttclient::on_HOSTNAMElineEdit_editingFinished()
{
    DataChanged_Flag |= HOST_NAME_CHANGED;
}

/**
 * @brief Libmqttclient::on_HOSTIPlineEdit_editingFinished
 */
void Libmqttclient::on_HOSTIPlineEdit_editingFinished()
{
    DataChanged_Flag |= SERVER_IP_CHANGED;
}

/**
 * @brief Libmqttclient::on_HOSTPORTlineEdit_editingFinished
 */
void Libmqttclient::on_HOSTPORTlineEdit_editingFinished()
{
    DataChanged_Flag |= SERVER_PORT_CHANGED;
}

/**
 * @brief Libmqttclient::on_USERNAMElineEdit_editingFinished
 */
void Libmqttclient::on_USERNAMElineEdit_editingFinished()
{
    DataChanged_Flag |= USER_NAME_CHANGED;
}

/**
 * @brief Libmqttclient::on_KEYWORDlineEdit_editingFinished
 */
void Libmqttclient::on_KEYWORDlineEdit_editingFinished()
{
    DataChanged_Flag |= USER_KEY_CHANGED;
}

/**
 * @brief Libmqttclient::on_SUBSCRIBElineEdit_editingFinished
 */
void Libmqttclient::on_SUBSCRIBElineEdit_editingFinished()
{
    /*not thing*/
}

/**
 * @brief Libmqttclient::on_PUBLISHlineEdit_editingFinished
 */
void Libmqttclient::on_PUBLISHlineEdit_editingFinished()
{
    /*not thing*/
}

/**
 * @brief Libmqttclient::on_HOSTNAMElineEdit_selectionChanged
 */
void Libmqttclient::on_HOSTNAMElineEdit_selectionChanged()
{
    emit show_keyboard(this ,ui->HOSTNAMElineEdit ,"输入服务器域名" ,ui->HOSTNAMElineEdit->text());
    this->hide();
}

/**
 * @brief Libmqttclient::on_HOSTIPlineEdit_selectionChanged
 */
void Libmqttclient::on_HOSTIPlineEdit_selectionChanged()
{
    emit show_keyboard(this ,ui->HOSTIPlineEdit ,"输入服务器IP" ,ui->HOSTIPlineEdit->text());
    this->hide();
}

/**
 * @brief Libmqttclient::on_HOSTPORTlineEdit_selectionChanged
 */
void Libmqttclient::on_HOSTPORTlineEdit_selectionChanged()
{
    emit show_keyboard(this ,ui->HOSTPORTlineEdit ,"输入服务器端口" ,ui->HOSTPORTlineEdit->text());
    this->hide();
}

/**
 * @brief Libmqttclient::on_USERNAMElineEdit_selectionChanged
 */
void Libmqttclient::on_USERNAMElineEdit_selectionChanged()
{
    emit show_keyboard(this ,ui->USERNAMElineEdit ,"输入用户名" ,ui->USERNAMElineEdit->text());
    this->hide();
}

/**
 * @brief Libmqttclient::on_KEYWORDlineEdit_selectionChanged
 */
void Libmqttclient::on_KEYWORDlineEdit_selectionChanged()
{
    emit show_keyboard(this ,ui->KEYWORDlineEdit ,"输入用户密码" ,ui->KEYWORDlineEdit->text());
    this->hide();
}

/**
 * @brief Libmqttclient::on_SUBSCRIBElineEdit_selectionChanged
 */
void Libmqttclient::on_SUBSCRIBElineEdit_selectionChanged()
{
    emit show_keyboard(this ,ui->SUBSCRIBElineEdit ,"输入订阅主题" ,ui->SUBSCRIBElineEdit->text());
    this->hide();
}

/**
 * @brief Libmqttclient::on_PUBLISHlineEdit_selectionChanged
 */
void Libmqttclient::on_PUBLISHlineEdit_selectionChanged()
{
    emit show_keyboard(this ,ui->PUBLISHlineEdit ,"输入发布主题" ,ui->PUBLISHlineEdit->text());
    this->hide();
}

/**
 * @brief Libmqttclient::on_CLEARpushButton_clicked
 */
void Libmqttclient::on_CLEARpushButton_clicked()
{
    ui->textBrowser->clear();
}

/**
 * @brief Libmqttclient::on_pushButton_clicked
 */
void Libmqttclient::on_SENDMSGpushButton_clicked()
{
    if(DebugMode == true && ui->DEBUGlineEdit->text().isEmpty() == false)
    {
        QString topicStr = ui->PUBLISHlineEdit->text();
//    PublishMsg(topicStr ,msg);
        qint32 ret = MqttClient->publish(topicStr ,ui->DEBUGlineEdit->text().toUtf8() ,static_cast<quint8>(ui->PUBLISHQOSspinBox->value()&0x000000FF));
        if(ret == 0)
        {
            CheckDebugbrowserIsFull();
            ui->textBrowser->append(orange_prefix +"--->" + "Send msg ok" + font_suffix);
            ui->textBrowser->moveCursor(QTextCursor::End);
        }
        else
        {
            CheckDebugbrowserIsFull();
            ui->textBrowser->append(red_prefix +"--->" + "Send msg error" + font_suffix);
            ui->textBrowser->moveCursor(QTextCursor::End);
        }
    }
}

/**
 * @brief Libmqttclient::CloseDebugMode
 */
void Libmqttclient::CloseDebugMode()
{
    ui->textBrowser->append(red_prefix + "已关闭调试." + font_suffix);
    ui->textBrowser->moveCursor(QTextCursor::End);
    ui->DEBUGlineEdit->setEnabled(false);
    ui->DEBUGlineEdit->setVisible(false);
    ui->DEBUGlineEdit->setText("");
    DebugMode = false;
}

/**
 * @brief Libmqttclient::OpenDebugMode
 */
void Libmqttclient::OpenDebugMode()
{
    ui->textBrowser->append(red_prefix + "已启用调试: 主题写入发布框,消息写入下方编辑框." + font_suffix);
    ui->textBrowser->moveCursor(QTextCursor::End);
    ui->DEBUGlineEdit->setEnabled(true);
    ui->DEBUGlineEdit->setVisible(true);
    DebugMode = true;
}

/**
 * @brief Libmqttclient::CheckDebugbrowser
 */
void Libmqttclient::CheckDebugbrowserIsFull()
{
    if(ui->textBrowser->isFullScreen() == true)
    {
        ui->textBrowser->clearHistory();
        ui->textBrowser->clear();
        ui->textBrowser->setText("");
    }
}

/**
 * @brief Libmqttclient::on_checkBox_stateChanged
 * @param arg1
 */
void Libmqttclient::on_checkBox_stateChanged(int arg1)
{
    if(arg1)
    {
        OpenDebugMode();
    }
    else
    {
        CloseDebugMode();
    }
}

/**
 * @brief Libmqttclient::on_DEBUGlineEdit_selectionChanged
 */
void Libmqttclient::on_DEBUGlineEdit_selectionChanged()
{
    emit show_keyboard(this ,ui->DEBUGlineEdit,"输入发布信息",ui->DEBUGlineEdit->text());
    this->hide();
}

/* ---------------------------- end of file ----------------------------------*/
