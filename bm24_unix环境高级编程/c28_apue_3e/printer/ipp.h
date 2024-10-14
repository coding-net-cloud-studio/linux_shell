#ifndef _IPP_H
#define _IPP_H

/*
 * Defines parts of the IPP protocol between the scheduler
 * and the printer.  Based on RFC2911 and RFC2910.
 */

/*
 * Status code classes.
 */
// 定义状态类宏,用于判断状态码的类别
// STATCLASS_OK(x) 判断状态码是否在 0x0000 到 0x00FF 之间,表示操作成功
#define STATCLASS_OK(x)     ((x) >= 0x0000 && (x) <= 0x00ff)

// STATCLASS_INFO(x) 判断状态码是否在 0x0100 到 0x01FF 之间,表示信息性消息
#define STATCLASS_INFO(x)   ((x) >= 0x0100 && (x) <= 0x01ff)

// STATCLASS_REDIR(x) 判断状态码是否在 0x0300 到 0x03FF 之间,表示重定向
#define STATCLASS_REDIR(x)  ((x) >= 0x0300 && (x) <= 0x03ff)

// STATCLASS_CLIERR(x) 判断状态码是否在 0x0400 到 0x04FF 之间,表示客户端错误
#define STATCLASS_CLIERR(x) ((x) >= 0x0400 && (x) <= 0x04ff)

// STATCLASS_SRVERR(x) 判断状态码是否在 0x0500 到 0x05FF 之间,表示服务器错误
#define STATCLASS_SRVERR(x) ((x) >= 0x0500 && (x) <= 0x05ff)

/*
 * Status codes.
 */
// ipp.h 文件中的状态码定义

// 成功状态码
#define STAT_OK             0x0000 /* 成功 */
#define STAT_OK_ATTRIGN     0x0001 /* 成功;一些属性被忽略 */
#define STAT_OK_ATTRCON     0x0002 /* 成功;一些属性冲突 */

// 客户端错误状态码
#define STAT_CLI_BADREQ     0x0400 /* 无效的客户端请求 */
#define STAT_CLI_FORBID     0x0401 /* 请求被禁止 */
#define STAT_CLI_NOAUTH     0x0402 /* 需要认证 */
#define STAT_CLI_NOPERM     0x0403 /* 客户端未授权 */
#define STAT_CLI_NOTPOS     0x0404 /* 请求不可能 */
#define STAT_CLI_TIMOUT     0x0405 /* 客户端太慢 */
#define STAT_CLI_NOTFND     0x0406 /* 没有为URI找到对象 */
#define STAT_CLI_OBJGONE    0x0407 /* 对象不再可用 */
#define STAT_CLI_TOOBIG     0x0408 /* 请求的实体太大 */
#define STAT_CLI_TOOLNG     0x0409 /* 属性值太大 */
#define STAT_CLI_BADFMT     0x040a /* 不支持的文档格式 */
#define STAT_CLI_NOTSUP     0x040b /* 不支持的属性 */
#define STAT_CLI_NOSCHM     0x040c /* 不支持的URI方案 */
#define STAT_CLI_NOCHAR     0x040d /* 不支持的字符集 */
#define STAT_CLI_ATTRCON    0x040e /* 属性冲突 */
#define STAT_CLI_NOCOMP     0x040f /* 不支持压缩 */
#define STAT_CLI_COMPERR    0x0410 /* 数据无法解压缩 */
#define STAT_CLI_FMTERR     0x0411 /* 文档格式错误 */
#define STAT_CLI_ACCERR     0x0412 /* 访问数据时出错 */

// 服务器错误状态码
#define STAT_SRV_INTERN     0x0500 /* 意外的内部错误 */
#define STAT_SRV_NOTSUP     0x0501 /* 操作不支持 */
#define STAT_SRV_UNAVAIL    0x0502 /* 服务不可用 */
#define STAT_SRV_BADVER     0x0503 /* 版本不支持 */
#define STAT_SRV_DEVERR     0x0504 /* 设备错误 */
#define STAT_SRV_TMPERR     0x0505 /* 临时错误 */
#define STAT_SRV_REJECT     0x0506 /* 服务器不接受作业 */
#define STAT_SRV_TOOBUSY    0x0507 /* 服务器太忙 */
#define STAT_SRV_CANCEL     0x0508 /* 作业已被取消 */
#define STAT_SRV_NOMULTI    0x0509 /* 不支持多文档作业 */

/*
 * Operation IDs
 */
// ipp.h

// 打印作业操作码
#define OP_PRINT_JOB        0x02
// 打印URI操作码
#define OP_PRINT_URI        0x03
// 验证作业操作码
#define OP_VALIDATE_JOB     0x04
// 创建作业操作码
#define OP_CREATE_JOB       0x05
// 发送文档操作码
#define OP_SEND_DOC         0x06
// 发送URI操作码
#define OP_SEND_URI         0x07
// 取消作业操作码
#define OP_CANCEL_JOB       0x08
// 获取作业属性操作码
#define OP_GET_JOB_ATTR     0x09
// 获取所有作业操作码
#define OP_GET_JOBS         0x0a
// 获取打印机属性操作码
#define OP_GET_PRINTER_ATTR 0x0b
// 暂停作业操作码
#define OP_HOLD_JOB         0x0c
// 释放作业操作码
#define OP_RELEASE_JOB      0x0d
// 重启作业操作码
#define OP_RESTART_JOB      0x0e
// 暂停打印机操作码
#define OP_PAUSE_PRINTER    0x10
// 恢复打印机操作码
#define OP_RESUME_PRINTER   0x11
// 清除所有作业操作码
#define OP_PURGE_JOBS       0x12

/*
 * Attribute Tags.
 */
// 定义操作属性标签
#define TAG_OPERATION_ATTR  0x01 /* 操作属性标签 */

// 定义作业属性标签
#define TAG_JOB_ATTR        0x02 /* 作业属性标签 */

// 定义属性结束标签
#define TAG_END_OF_ATTR     0x03 /* 属性结束标签 */

// 定义打印机属性标签
#define TAG_PRINTER_ATTR    0x04 /* 打印机属性标签 */

// 定义不支持的属性标签
#define TAG_UNSUPP_ATTR     0x05 /* 不支持的属性标签 */

/*
 * Value Tags.
 */
// ipp.h 文件中的标签定义,用于标识不同的数据类型或值

// 不支持的值
#define TAG_UNSUPPORTED     0x10
// 未知的值
#define TAG_UNKNOWN         0x12
// 无值
#define TAG_NONE            0x13
// 整数值
#define TAG_INTEGER         0x21
// 布尔值
#define TAG_BOOLEAN         0x22
// 枚举值
#define TAG_ENUM            0x23
// 八位字符串
#define TAG_OCTSTR          0x30
// 日期时间
#define TAG_DATETIME        0x31
// 分辨率
#define TAG_RESOLUTION      0x32
// 整数范围
#define TAG_INTRANGE        0x33
// 带语言的文本
#define TAG_TEXTWLANG       0x35
// 带语言的名称
#define TAG_NAMEWLANG       0x36
// 不带语言的文本
#define TAG_TEXTWOLANG      0x41
// 不带语言的名称
#define TAG_NAMEWOLANG      0x42
// 关键字
#define TAG_KEYWORD         0x44
// 统一资源标识符
#define TAG_URI             0x45
// URI 方案
#define TAG_URISCHEME       0x46
// 字符集
#define TAG_CHARSET         0x47
// 自然语言
#define TAG_NATULANG        0x48
// MIME 类型
#define TAG_MIMETYPE        0x49

struct ipp_hdr
{
    int8_t major_version;  // 主版本号,始终为1
    int8_t minor_version;  // 次版本号,始终为1
    union
    {
        int16_t op;         // 操作ID
        int16_t st;         // 状态
    } u;                    // 联合体,用于操作ID或状态
    int32_t request_id;     // 请求ID
    char    attr_group[1];  // 可选属性组的起始位置
                            // 后续跟随可选数据
};

// 定义宏operation,用于获取结构体u中的op成员
#define operation u.op

// 定义宏status,用于获取结构体u中的st成员
#define status    u.st

#endif /* _IPP_H */
