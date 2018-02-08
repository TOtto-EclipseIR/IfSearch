#ifndef SINGLETON_H
#define SINGLETON_H

#define DECLARE_SINGLETON(CLASS) \
    protected: \
        CLASS(void); \
    public: \
        static CLASS * instance(void); \
    private: \
        static CLASS * class_pointer; \

#define DEFINE_SINGLETON(CLASS) \
    CLASS * CLASS::class_pointer = 0; \
    CLASS * CLASS::instance(void) \
    { \
        if ( ! class_pointer) \
            class_pointer = new CLASS; \
        return class_pointer; \
    } \

#endif // SINGLETON_H
