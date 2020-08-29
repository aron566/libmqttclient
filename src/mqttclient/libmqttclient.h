/**
 *  @file Libmqttclient.h
 *
 *  @date 2020-08-25
 *
 *  @author aron566
 *
 *  @brief Mqtt客户端
 *
 *  @version V1.0
 */
#ifndef LIBMQTTCLIENT_H
#define LIBMQTTCLIENT_H
/** Includes -----------------------------------------------------------------*/
#include "libmqttclient_global.h"
#include <QWidget>
#include <QLineEdit>
#include <QHostInfo>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QTimer>
#include <QProcess>

/** Private includes ---------------------------------------------------------*/
#include <qmqttclient.h>
#include <qmqtttopicname.h>
#include <qmqttsubscription.h>

/** Private defines ----------------------------------------------------------*/
namespace Ui {
class libmqttclient;
}
/** Exported typedefines -----------------------------------------------------*/

/** Exported constants -------------------------------------------------------*/

/** Exported macros-----------------------------------------------------------*/
/** Exported variables -------------------------------------------------------*/
/** Exported functions prototypes --------------------------------------------*/

/**
 * @brief The mqtt class
 */
class LIBMQTTCLIENT_EXPORT Libmqttclient : public QWidget
{
    Q_OBJECT
public:
    enum DATA_TYPE
    {
        HOST_NAME = 0,
        HOST_IP,
        HOST_PORT,
        USER_NAME,
        USER_KEY,
        SUB_TOPIC,
        PUB_TOPIC,
    };
public:
    explicit Libmqttclient(int width ,int height ,QWidget *parent = nullptr);
    explicit Libmqttclient(int width ,int height ,QString ip
            ,bool usessl = false
            ,quint8 clientnum = 0
            ,QWidget *parent = nullptr
            ,QString addr = MqttServerAddr
            ,QString username = MqttUserName
            ,QString key = MqttUserKey
            ,quint16 port = MqttServerPort
            ,quint16 keepalive = MqttKeepAlive);
    ~Libmqttclient();

signals:
    void MqttwindowIsClose();                                    /**< 通知父窗口显示*/
    void show_keyboard(QWidget * ,QLineEdit *,QString,QString);  /**< 通知显示键盘*/
    void NetworkPingError();                                     /**< 网络连接错误*/
    void MessageReceived(const QByteArray &message, const QMqttTopicName &topic = QMqttTopicName());/**< 接收到新消息信号*/
    void StartReconnectTimer();
private slots:
    void on_CONNECTpushButton_clicked();

    void on_SUBSCRIBEpushButton_clicked();

    void on_SUBSCRIBEQOSspinBox_valueChanged(int arg1);

    void on_PUBLISHpushButton_clicked();

    void on_PUBLISHQOSspinBox_valueChanged(int arg1);

    void on_returnpushButton_clicked();

    void on_confirmpushButton_clicked();

    void on_HOSTNAMElineEdit_editingFinished();

    void on_HOSTIPlineEdit_editingFinished();

    void on_HOSTPORTlineEdit_editingFinished();

    void on_USERNAMElineEdit_editingFinished();

    void on_KEYWORDlineEdit_editingFinished();

    void on_SUBSCRIBElineEdit_editingFinished();

    void on_PUBLISHlineEdit_editingFinished();

    void on_HOSTNAMElineEdit_selectionChanged();

    void on_HOSTIPlineEdit_selectionChanged();

    void on_HOSTPORTlineEdit_selectionChanged();

    void on_USERNAMElineEdit_selectionChanged();

    void on_KEYWORDlineEdit_selectionChanged();

    void on_SUBSCRIBElineEdit_selectionChanged();

    void on_PUBLISHlineEdit_selectionChanged();

    void on_CLEARpushButton_clicked();

    void on_UNSUBSCRIBEpushButton_clicked();

    void on_UNPUBLISHpushButton_clicked();

    void on_SENDMSGpushButton_clicked();

    void on_checkBox_stateChanged(int arg1);

    void on_DEBUGlineEdit_selectionChanged();
private slots:
    void lookedUp(const QHostInfo &host);   /**< 处理转换后返回的域名对应IP地址*/
    void slotConnected();                   /**< 响应已连接信号*/
    void slotDisconnected();                /**< 响应断连信号*/
    void slotPingResponse();                /**< 响应ping有响应信号*/
    void slotMessageReceived(const QByteArray &message, const QMqttTopicName &topic = QMqttTopicName());/**< 响应新消息信号*/
    void slotSessionRestored();             /**< 响应断连恢复信号*/
    void DealTimeout();                     /**< 自启动连接定时器*/
    void DealDisconnectTimeout();           /**< 自重连定时器，读取输入框内容进行重连*/
    void slotexeFinished();                 /**< ping 检测结果*/
    void slotauthenticationRequested(const QMqttAuthenticationProperties &p);
    void slotauthenticationFinished(const QMqttAuthenticationProperties &p);

public:
    static bool NetworkConnectCheck(QString netaddr);   /**< 检测与指定外网的连接*/
    void SetReconnectInterval(qint32 msec);             /**< 设置重连检测周期*/
    void NetworkPingCheck(QString netaddr);             /**< 检测指定ip是否能够ping通*/
    void SwitchClientConfig(quint8 clientNum = 0);      /**< 切换客户端配置*/
    void setProtocolVersion(QMqttClient::ProtocolVersion protocolVersion);/**< 设置版本*/
    QMqttClient::ClientState GetMqttConnectState()const;/**< 获取mqtt连接状态 0未连接 1连接中 2已连接*/
    void UNsubscribe_all();                             /**< 取消全部订阅*/
    void UNpublish_all();                               /**< 取消全部发布*/
    qint32 PublishMsg(QString topicStr ,QByteArray msg);/**< 发布消息*/
    bool RemoveSubscribeToic(QString topic ,bool Writeflash = false ,quint8 configindex = 0);               /**< 删除指定订阅,选择是否写入到配置*/
    bool RemovePublishToic(QString topic ,bool Writeflash = false ,quint8 configindex = 0);                 /**< 删除指定发布,选择是否写入到配置*/
    bool AddSubscribeToic(QString topic ,qint32 qosvalue ,bool Writeflash = false ,quint8 configindex = 0); /**< 新增指定订阅,选择是否写入到配置*/
    bool AddPublishToic(QString topic ,qint32 qosvalue ,bool Writeflash = false ,quint8 configindex = 0);   /**< 新增指定发布,选择是否写入到配置*/
private:
    void PreSetConfig(quint8 clientnum ,QString addr ,QString ip ,QString username ,QString key ,quint16 port ,quint16 keepalive);
    void ConfigParInit();
    void ConfigFlagInit();
    void MqttConfigInit();
    void Create_ConfigFile();
    bool WriteJsonFile(QJsonObject &rootObj);
    bool ReadConfigFromJson(QJsonObject &rootObj ,bool updateConfig = true);
    bool ConnectToMqttServer(bool useEditbox = true);
    bool DisconnectToMqttServer();
    bool ModifiedJson(QJsonObject &rootObj ,quint8 configindex ,QString id ,bool value);
    bool ModifiedJson(QJsonObject &rootObj ,quint8 configindex ,QString id ,QString value);
    bool ModifiedJson(QJsonObject &rootObj ,quint8 configindex ,QString id ,QString value ,QString topic)QT_DEPRECATED_X("Use ModifiedJsonRemoveTopic()instead");/**< 仅适用于topic修改*/

    bool ModifiedJsonTopic(QJsonObject &rootObj ,quint8 configindex ,QString id ,DATA_TYPE type);
    bool ModifiedJsonRemoveTopic(QJsonObject &rootObj ,quint8 configindex ,QString id ,QString topic ,DATA_TYPE type);/**< 仅适用于topic与qos移除*/
    bool ModifiedJsonAddTopic(QJsonObject &rootObj ,quint8 configindex ,QString id ,QString value ,QString qos ,qint32 qosvalue ,DATA_TYPE type);/**< 仅适用于topic与qos增加*/
    bool ModifiedJson(QJsonObject &rootObj ,quint8 configindex ,QString id ,qint32 value);
    void Modified_Config(QJsonObject &ConfigrootObj ,DATA_TYPE type);
    void MqttConfigRestore();/**< mqtt参数配置恢复*/
    bool AddSubscribe(QString topicStr ,bool manual_flag = false);/**< 增加一条订阅，如果是手动模式若出错则显示出错对话框*/
    bool DeleteSubscribe(QString topicStr ,bool manual_flag = false);
    bool AddPublish(QString topicStr ,bool manual_flag = false);/**< 增加一条发布，如果是手动模式若出错则显示出错对话框*/
    bool DeletePublish(QString topicStr ,bool manual_flag = false);
    void CloseDebugMode();
    void OpenDebugMode();
private:
    Ui::libmqttclient *ui;
    QTimer *NetworkCheckTimer;
    QTimer *ReconnectTimer;
    QMqttClient *MqttClient;
    QProcess Exec;
public:
    static QString MqttClentID;
    static QString MqttServerAddr;
    static QString MqttServerIp;
    static quint16 MqttServerPort;
    static QString MqttUserName;
    static QString MqttUserKey;
    static quint16 MqttKeepAlive;
    static QStringList MqttSubscribeTopic;
    static QList<qint32>MqttSUBQos;
    static QStringList MqttPublishTopic;
    static QList<qint32>MqttPUBQos;
private:
    bool UseSSL = false;/**< 使用ssl*/
#if defined(Q_CC_MINGW)
    QSslConfiguration sslConfig;
#endif
    quint8 ActiveClientNum = 0;/**< 激活指定客户端*/
    quint32 DataChanged_Flag = 0;
    bool MqttConnectState = false;
    bool AUTO_Connect_Active = true;
    QString PINGRetstr;
    quint8 ConnectErrorCnt = 0;
    bool DebugMode = false;
};
#endif // LIBMQTTCLIENT_H

/******************************** End of file *********************************/
