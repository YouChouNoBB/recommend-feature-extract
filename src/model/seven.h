//
// Created by Bryan Zhou on 2022/10/25.
//

#ifndef EXTRACTOR_SEVEN_H
#define EXTRACTOR_SEVEN_H

template<class TT1, class TT2, class TT3, class TT4, class TT5, class TT6, class TT7>
class Seven {
public:

    Seven<TT1, TT2, TT3, TT4, TT5,TT6,TT7> make_five(TT1 t1, TT2 t2, TT3 t3, TT4 t4, TT5 t5, TT6 t6, TT7 t7) {
        return Seven(t1, t2, t3, t4, t5,t6,t7);
    }

    Seven(TT1 t1, TT2 t2, TT3 t3, TT4 t4, TT5 t5,TT6 t6, TT7 t7) : first(t1), second(t2), third(t3), fourth(t4), fifth(t5),six(t6),seven(t7) {}

    ~Seven() {}

    Seven() {
        first=0;
        second=0;
        third=0;
        fourth=0;
        fifth=0;
        six=0;
        seven=0;
    }

    void setFirst(TT1 first) { this->first = first; }

    void setSecond(TT2 second) {this->second = second;}

    void setThird(TT3 third) {this->third = third;}

    void setFourth(TT4 fourth) {this->fourth = fourth;}

    void setFifth(TT5 fifth) {this->fifth = fifth;}

    void setSix(TT6 t6) {this->six = t6;}

    void setSeven(TT6 t7) {this->seven = t7;}

    TT1 first;
    TT2 second;
    TT3 third;
    TT4 fourth;
    TT5 fifth;
    TT6 six;
    TT7 seven;
};

#endif //EXTRACTOR_SEVEN_H
