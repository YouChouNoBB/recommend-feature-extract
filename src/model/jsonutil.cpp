//
// Created by Bryan Zhou on 2022/10/26.
//
#include "jsonutil.h"
bool proto_to_json(const google::protobuf::Message& message, std::string& json) {
    google::protobuf::util::JsonPrintOptions options;
    options.add_whitespace = true;
    options.always_print_primitive_fields = true;
    options.preserve_proto_field_names = true;
    return MessageToJsonString(message, &json, options).ok();
}

bool json_to_proto(const std::string& json, google::protobuf::Message& message) {
    return JsonStringToMessage(json, &message).ok();
}

bool proto_to_string(const google::protobuf::Message& message,std::string *s){
    return google::protobuf::TextFormat::PrintToString(message,s);
}