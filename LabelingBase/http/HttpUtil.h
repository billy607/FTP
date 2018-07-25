#ifndef LABELINGHTTPUTIL_H
#define LABELINGHTTPUTIL_H

#include"labelingbase_global.h"
#include"task/OnlineTask.h"
#include"context/Context.h"
#include<QCryptographicHash>
#include<QJsonObject>
#include<QJsonArray>
#include"QJsonDocument"
#include"LogUtil.h"
#include"LogEvent.h"
#include"labelingbase_global.h"
namespace labelingbase {



class ManagerContext;
class ToolContext;
class LABELINGBASESHARED_EXPORT HttpUtil
{
public:
    HttpUtil();
    ~HttpUtil();


    /**
     * @brief login 登录以获得token
     * @param context ，应用上下文
     * @return 成功与否
     */
    bool login(ManagerContext * context);
    bool login(ToolContext * context);



    /**
     * @brief getOnlinePackets ：获得在线任务数据，tool应用通过taskId，和packageId，筛选当前应用，manager保存所有任务状态
     * @param context ： 应用上下文
     * @return 成功与否
     */

    bool initTaskInfomation(ToolContext * context, QString taskId, QString packageId );


    /**
     * @brief uploadLogEvent 上传事件日志
     * @param context 应用上下文
     * @param taskId 任务id
     * @param packageId 包号
     * @param event 事件信息
     * @return 成功与否
     */

    bool uploadLogEvent(ManagerContext *, QString taskId, QString packageId  ,LogEvent &event);
    bool uploadLogEvent(ToolContext * ,LogEvent &event);




    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /**
     * 以下操作只能被Manager应用调用
     */

    /**
     * @brief downloadPackage 下载数据包
     * @param context 应用上下文
     * @param taskId 任务id
     * @param packageId 包号
     * @param progress 下载进程回调函数
     * @param param 回调函数参数
     * @return 成功与否
     */

    bool downloadPackage(ManagerContext * context , QString taskId, QString packageId , loadProgress progress , void * param);


    /**
     * @brief downloadPackage 下载数据包
     * @param context 应用上下文
     * @param taskId 任务id
     * @param packageId 包号
     * @param progress 下载进程回调函数
     * @param param 回调函数参数
     * @return 成功与否
     */

    bool downloadPatchPackage(ManagerContext * context , QString taskId, QString packageId );


    /**
     * @brief downLoadToolPackage 下载在线工具
     * @param context 应用上下文
     * @param toolID 工具id
     * @param progress 下载进程回调函数
     * @param param 下载进程回调参数
     * @return 成功与否
     */

    bool downLoadToolPackage(ManagerContext *context , QString taskId, loadProgress progress , void * param);

    /**
     * @brief downLoadToolPackage 下载在线工具
     * @param context 应用上下文
     * @param toolID 工具id
     * @param progress 下载进程回调函数
     * @param param 下载进程回调参数
     * @return 成功与否
     */

    bool downLoadToolPackageByToolId(ManagerContext *context , QString toolId, loadProgress progress , void * param);


    /**
     * @brief downloadConfigureFile 下载配置文件
     * @param context 应用上下文
     * @param taskId 任务id
     * @return 成功与否
     */
    bool downloadConfigureFile(ManagerContext *context, QString taskId) ;

    /**
     * @brief getTooInfomation 获得工具参数信息，包括校验和等
     * @param context 应用上下文
     * @param toolId 工具id
     * @return 成功与否
     */
    bool getTooInfomation(ManagerContext * context  , QString toolId);



    /**
     * @brief getOnlinePackets ：获得所有在线任务
     * @param context ： 应用上下文
     * @return 成功与否
     */
    bool initOnlinePackets(ManagerContext * context);


    /**
     * @brief uploadLabelingResult 上传结果
     * @param context 应用上下文
     * @param taskId 任务id
     * @param packageId 包id
     * @return 成功与否
     */
    bool uploadLabelingResult(ManagerContext *context, QString taskId, QString packageId,bool success = true) ;




};

}




#endif // LABELINGHTTPUTIL_H
