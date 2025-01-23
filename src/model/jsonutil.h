//
// Created by Bryan Zhou on 2022/10/25.
//

#ifndef EXTRACTOR_JSONUTIL_H
#define EXTRACTOR_JSONUTIL_H

#include <google/protobuf/message.h>
#include <google/protobuf/util/json_util.h>
#include <google/protobuf/text_format.h>
using google::protobuf::util::JsonStringToMessage;
/**
 * pb协议的数据转json格式
 * @param message  pb数据
 * @param json 存放json的字符串
 * @return bool
 */
bool proto_to_json(const google::protobuf::Message& message, std::string& json);

/**
 * json转pb
 * @param json
 * @param message
 * @return
 */
bool json_to_proto(const std::string& json, google::protobuf::Message& message);

/**
 * pb协议的数据格式化成string，一般调用这个方法
 * @param message
 * @param s
 * @return
 */
bool proto_to_string(const google::protobuf::Message& message,std::string* s);

/*
 *
 * string转byte[]:
byte[] byteArray = System.Text.Encoding.Default.GetBytes ( str );

byte[]转string：
string str = System.Text.Encoding.Default.GetString ( byteArray );

string转ASCII byte[]:
byte[] byteArray = System.Text.Encoding.ASCII.GetBytes ( str );

ASCII byte[]转string:
string str = System.Text.Encoding.ASCII.GetString ( byteArray );
 * */


#endif //EXTRACTOR_JSONUTIL_H
