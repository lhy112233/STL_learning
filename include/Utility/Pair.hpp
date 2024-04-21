# ifndef PAIR_HPP_
#define PAIR_HPP_


namespace hy{
    template <typename T1,typename T2>
    class pair{
        T1 first;
        T2 second;

        public:
        using first_type = T1;
        using second_type = T2;

        constexpr explicit pair() = default;
        constexpr pair(const T1& arg1,const T2& arg2) : first{arg1} , second{arg2}{}

        
    };

}




#endif  //PAIR_HPP_