//
// Created by Bryan Zhou on 2022/10/25.
//

#ifndef EXTRACTOR_FIVE_H
#define EXTRACTOR_FIVE_H

template<class TT1, class TT2, class TT3, class TT4, class TT5>
class Five {
public:

    Five<TT1, TT2, TT3, TT4, TT5> make_five(TT1 t1, TT2 t2, TT3 t3, TT4 t4, TT5 t5) {
        return Five(t1, t2, t3, t4, t5);
    }

    Five(TT1 t1, TT2 t2, TT3 t3, TT4 t4, TT5 t5) : first(t1), second(t2), third(t3), fourth(t4), fifth(t5) {}

    ~Five() {}

    Five() {
        first=0;
        second=0;
        third=0;
        fourth=0;
        fifth=0;
    }

    void setFirst(TT1 first) { this->first = first; }

    void setSecond(TT2 second) {this->second = second;}

    void setThird(TT3 third) {this->third = third;}

    void setFourth(TT4 fourth) {this->fourth = fourth;}

    void setFifth(TT5 fifth) {this->fifth = fifth;}

    TT1 first;
    TT2 second;
    TT3 third;
    TT4 fourth;
    TT5 fifth;
};

#endif //EXTRACTOR_FIVE_H
