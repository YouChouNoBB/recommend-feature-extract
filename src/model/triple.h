//
// Created by Bryan Zhou on 2022/10/24.
//

#ifndef EXTRACTOR_TRIPLE_H
#define EXTRACTOR_TRIPLE_H
template <class _T1, class _T2,class _T3>
class Triple{
public:
    Triple<_T1,_T2,_T3> make_triple(_T1 t1,_T2 t2,_T3 t3){
        return Triple( t1, t2, t3);
    }
    Triple(_T1 t1,_T2 t2,_T3 t3):first(t1),second(t2),third(t3){
    }
    ~Triple(){

    }

    _T1 first;
    _T2 second;
    _T3 third;

};
#endif //EXTRACTOR_TRIPLE_H
