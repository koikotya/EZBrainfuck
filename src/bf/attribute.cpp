#include "attribute.hpp"

Attribute::Attribute() {}

Type castType(Type t1,Type t2) {
    return (t1 > t2 ? t1 : t2);
}
