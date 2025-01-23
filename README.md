# recommend-feature-extract

feature-extract with cpp

## develop
- application 应用层
- features    特征注册
- model       模型层
- pb          协议层
- tables      抽取逻辑层

## mac build
```
mkdir build
cd build
rm -rf *ake*
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j4
```

## linux build
```
mkdir build
cd build
rm -rf *ake*
cmake .. -DCMAKE_CXX_COMPILER=/usr/local/bin/g++ -DCMAKE_C_COMPILER=/usr/local/bin/gcc -DCMAKE_BUILD_TYPE=Release
make -j16
aws s3 cp /mnt/code/recommend-feature-extract/lib/libextract.so s3://mlalgorithm/lib/
```

## run
```
export OMP_NUM_THREADS=4
export LD_LIBRARY_PATH=/usr/local/lib:./
./extractor

error while loading shared libraries: libprotobuf.so.32: cannot open shared object file: No such file or directory
sudo aws s3 cp s3://mlalgorithm/jars/libprotobuf.so /usr/local/lib

/lib64/libstdc++.so.6: version `GLIBCXX_3.4.?' not found,https://blog.csdn.net/qq171563857/article/details/80924353
sudo aws s3 cp s3://mlalgorithm/jars/libstdc++.so.6 /usr/lib64/

export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
./extractor

```

## lib
```
../lib/extract.so
```


## make jni
```
https://blog.csdn.net/danielpei1222/article/details/62462497
https://zhuanlan.zhihu.com/p/554439122
1.编写java接口，包含native方法的文件 JNIExtractor.java
2.编译生成 JNIExtractor.class文件
javac JNIExtractor.java
3.利用javah命令生成 c++ 头文件
javah -classpath /mnt/code/shoplazza-recsys/recsys-common/src/main/java -d ./ -jni com.shoplazza.algorithm.common.feature.model.JNIExtractor
4.根据.h文件编写.cpp文件
5.编译出.so文件
6.spark配置,实测需要用--files
--conf spark.executor.extraLibraryPath=s3://mlalgorithm/lib
或者
--files s3://mlalgorithm/lib/libextract.so,s3://mlalgorithm/lib/libprotobuf.so.32,s3://mlalgorithm/lib/libprotobuf.so,s3://mlalgorithm/lib/libstdc++.so.6 \

遇到的问题参考
1）byte* jbyteArray转换
https://blog.csdn.net/qq_38074673/article/details/102840760
2）spark加载so
https://stackoverflow.com/questions/45741082/loading-shared-libraries-so-distributed-by-files-argument-with-spark
https://stackoverflow.com/questions/52531108/how-to-reference-so-files-in-spark-submit-command?noredirect=1&lq=1

```