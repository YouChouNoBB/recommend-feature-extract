//
// Created by Bryan Zhou on 2022/10/26.
//

#ifndef EXTRACTOR_EDIT_DISTANCE_H
#define EXTRACTOR_EDIT_DISTANCE_H
#include <string>
#include <cmath>
using namespace std;
class EditDistance{
public:
    static int minDistance(string sourceStr, string targetStr){
        int sourceLen = sourceStr.length();
        int targetLen = targetStr.length();

        if(sourceLen == 0){
            return targetLen;
        }
        if(targetLen == 0){
            return sourceLen;
        }

        //定义矩阵(二维数组)
        int arr[sourceLen+1][targetLen+1];


        for(int i=0; i < sourceLen+1; i++){
            arr[i][0] = i;
        }
        for(int j=0; j < targetLen+1; j++){
            arr[0][j] = j;
        }

        char sourceChar ;
        char targetChar ;

        for(int i=1; i < sourceLen+1 ; i++){
            sourceChar = sourceStr[i-1];

            for(int j=1; j < targetLen+1 ; j++){
                targetChar = targetStr[j-1];

                if(sourceChar==targetChar){
                    /*
                     *  如果source[i] 等于target[j]，则：d[i, j] = d[i-1, j-1] + 0          （递推式 1）
                     */
                    arr[i][j] = arr[i-1][j-1];
                }else{
                    /*  如果source[i] 不等于target[j]，则根据插入、删除和替换三个策略，分别计算出使用三种策略得到的编辑距离，然后取最小的一个：
                        d[i, j] = min(d[i, j - 1] + 1, d[i - 1, j] + 1, d[i - 1, j - 1] + 1 )    （递推式 2）
                        >> d[i, j - 1] + 1 表示对source[i]执行插入操作后计算最小编辑距离
                        >> d[i - 1, j] + 1 表示对source[i]执行删除操作后计算最小编辑距离
                        >> d[i - 1, j - 1] + 1表示对source[i]替换成target[i]操作后计算最小编辑距离
                    */
                    arr[i][j] = (min(min(arr[i-1][j], arr[i][j-1]), arr[i-1][j-1])) + 1;
                }
            }
        }

        return arr[sourceLen][targetLen];
    }

    /**
     * 计算字符串相似度
     * similarity = (maxlen - distance) / maxlen
     * ps: 数据定义为double类型,如果为int类型 相除后结果为0(只保留整数位)
     * @param str1
     * @param str2
     * @return
     */
    static float getsimilarity(string str1,string str2){
        double distance = minDistance(str1,str2);
        double maxlen = max(str1.length(),str2.length());
        double res = (maxlen - distance)/maxlen;
        if(res==0.0)
            res=1e-6;
        return (float)res;
    }
};
#endif //EXTRACTOR_EDIT_DISTANCE_H
