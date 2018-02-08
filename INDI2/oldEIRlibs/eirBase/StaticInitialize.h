#ifndef STATICINITIALIZE_H
#define STATICINITIALIZE_H

#define DECLARE_STATICINITIALIZE(CLASS) \
    private:    \
        enum initialzer { initialize }; \
        void staticInitialize(void); \
        CLASS(initialzer i) { staticInitialize(); } \
        static CLASS dummy; \

#define DEFINE_STATICINITIALIZE(CLASS) \
        CLASS CLASS::dummy(CLASS::initialize); \

#endif // STATICINITIALIZE_H
