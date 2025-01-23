//
// Created by Bryan Zhou on 2022/10/25.
//

#ifndef EXTRACTOR_VECTOR_H
#define EXTRACTOR_VECTOR_H


#include <vector>
#include <cmath>
using namespace std;
class Vector{

public:
    Vector(vector<float> values):values(values){}
    ~Vector(){values.clear();}

    /**
     * 内积
     * @param v
     * @return
     */
    double dot(Vector v){
        vector<float> vs = v.values;
        double sum=0;
        for(int i=0;i<values.size();++i){
            sum+= values[i]*vs[i];
        }
        return sum;
    }

    /**
     * norm2
     * @return
     */
    double norm2(){
        double sum = 0.0;
        for(float v:values){
            sum+= v*v;
        }
        return std::sqrt(sum);
    }

    float cosSim(Vector v){
        float sim = 1e-6f;
        double norm1 = v.norm2();
        double norm2 = this->norm2();
        if(norm1*norm2==0.0) return sim;
        sim = (float)(dot(v) / (norm1*norm2));
        if(sim==0.0)
            sim=1e-6f;
        return sim;
    }


    vector<float> values;
};



#endif //EXTRACTOR_VECTOR_H
