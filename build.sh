g++ -std=c++20 src/application/main.cc \
src/model/sample.cpp \
src/model/tfrecord.cpp \
src/model/table_extractor.cpp \
src/model/extractor.cpp \
src/pb/features.pb.cc \
src/pb/tfexample.pb.cc \
src/application/rcmd/collection_v10.cpp \
-o main_test \
-I /usr/local/protobuf/include/ \
-I src/pb/ \
-I src/model/ \
-I src/features/ \
-I src/tables/context \
-I src/tables/cross \
-I src/tables/ctxitem \
-I src/tables/item \
-I src/tables/query \
-I src/tables/user \
-L /usr/local/protobuf/lib/ \
-lpthread \
-lprotobuf