#pragma once
//------------------------------------------------------------------------------
/**
    @class Util::Variant

    An "any type" variable.

    Since the Variant class has a rich set of assignment and cast operators,
    a variant variable can most of the time be used like a normal C++ variable.
    
    (C) 2006 RadonLabs GmbH.
*/
#include "math/float4.h"
#include "math/matrix44.h"
#include "util/string.h"
#include "util/guid.h"
#include "util/blob.h"
#include "memory/memory.h"
#include "core/refcounted.h"

//------------------------------------------------------------------------------
namespace Util
{
class Variant
{
public:
    /// variant types
    enum Type
    {
        Void,
        Int,
        Float,
        Bool,
        Float4,
        String,
        Matrix44,
        Blob,
        Guid,
        Object,
        IntArray,
        FloatArray,
        BoolArray,
        Float4Array,
        StringArray,
        Matrix44Array,
        BlobArray,
        GuidArray,
    };

    /// default constructor
    Variant();
    /// int constructor
    Variant(int rhs);
    /// float constructor
    Variant(float rhs);
    /// bool constructor
    Variant(bool rhs);
    /// float4 constructor
    Variant(const Math::float4& v);
    /// matrix44 constructor
    Variant(const Math::matrix44& m);
    /// string constructor
    Variant(const Util::String& rhs);
    /// blob constructor
    Variant(const Util::Blob& blob);
    /// guid constructor
    Variant(const Util::Guid& guid);
    /// const char constructor
    Variant(const char* chrPtr);
    /// object constructor
    Variant(Core::RefCounted* ptr);
    /// int array constructor
    Variant(const Util::Array<int>& rhs);
    /// float array constructor
    Variant(const Util::Array<float>& rhs);
    /// bool array constructor
    Variant(const Util::Array<bool>& rhs);
    /// float4 array constructor
    Variant(const Util::Array<Math::float4>& rhs);
    /// matrix44 array constructor
    Variant(const Util::Array<Math::matrix44>& rhs);
    /// string array constructor
    Variant(const Util::Array<Util::String>& rhs);
    /// blob array constructor
    Variant(const Util::Array<Util::Blob>& rhs);
    /// guid array constructor
    Variant(const Util::Array<Util::Guid>& rhs);
    /// copy constructor
    Variant(const Variant& rhs);

    /// destructor
    ~Variant();
    /// set type of attribute
    void SetType(Type t);
    /// get type
    Type GetType() const;
    /// clear content, resets type to void
    void Clear();
    
    /// assignment operator
    void operator=(const Variant& rhs);
    /// int assignment operator
    void operator=(int val);
    /// float assignment operator
    void operator=(float val);
    /// bool assigment operator
    void operator=(bool val);
    /// float4 assignment operator
    void operator=(const Math::float4& val);
    /// matrix44 assignment operator
    void operator=(const Math::matrix44& val);
    /// string assignment operator
    void operator=(const Util::String& s);
    /// blob assignment operator
    void operator=(const Util::Blob& val);
    /// guid assignment operator
    void operator=(const Util::Guid& val);
    /// char pointer assignment
    void operator=(const char* chrPtr);
    /// object assignment
    void operator=(Core::RefCounted* ptr);
    /// int array assignment
    void operator=(const Util::Array<int>& rhs);
    /// float array assignment
    void operator=(const Util::Array<float>& rhs);
    /// bool array assignment
    void operator=(const Util::Array<bool>& rhs);
    /// float4 array assignment
    void operator=(const Util::Array<Math::float4>& rhs);
    /// matrix44 array assignment
    void operator=(const Util::Array<Math::matrix44>& rhs);
    /// string array assignment
    void operator=(const Util::Array<Util::String>& rhs);
    /// blob array assignment
    void operator=(const Util::Array<Util::Blob>& rhs);
    /// guid array assignment
    void operator=(const Util::Array<Util::Guid>& rhs);

    /// equality operator
    bool operator==(const Variant& rhs) const;
    /// int equality operator
    bool operator==(int rhs) const;
    /// float equality operator
    bool operator==(float rhs) const;
    /// bool equality operator
    bool operator==(bool rhs) const;
    /// float4 equality operator
    bool operator==(const Math::float4& rhs) const;
    /// string equality operator
    bool operator==(const Util::String& rhs) const;
    /// guid equality operator
    bool operator==(const Util::Guid& rhs) const;
    /// char ptr equality operator
    bool operator==(const char* chrPtr) const;
    /// pointer equality operator
    bool operator==(Core::RefCounted* ptr) const;

    /// inequality operator
    bool operator!=(const Variant& rhs) const;
    /// int inequality operator
    bool operator!=(int rhs) const;
    /// float inequality operator
    bool operator!=(float rhs) const;
    /// bool inequality operator
    bool operator!=(bool rhs) const;
    /// float4 inequality operator
    bool operator!=(const Math::float4& rhs) const;
    /// string inequality operator
    bool operator!=(const Util::String& rhs) const;
    /// guid inequality operator
    bool operator!=(const Util::Guid& rhs) const;
    /// char ptr inequality operator
    bool operator!=(const char* chrPtr) const;
    /// pointer equality operator
    bool operator!=(Core::RefCounted* ptr) const;

    /// greater operator
    bool operator>(const Variant& rhs) const;
    /// less operator
    bool operator<(const Variant& rhs) const;
    /// greater equal operator
    bool operator>=(const Variant& rhs) const;
    /// less equal operator
    bool operator<=(const Variant& rhs) const;

    /// set integer content
    void SetInt(int val);
    /// get integer content
    int GetInt() const;
    /// set float content
    void SetFloat(float val);
    /// get float content
    float GetFloat() const;
    /// set bool content
    void SetBool(bool val);
    /// get bool content
    bool GetBool() const;
    /// set string content
    void SetString(const Util::String& val);
    /// get string content
    const Util::String& GetString() const;
    /// set float4 content
    void SetFloat4(const Math::float4& val);
    /// get float4 content
    Math::float4 GetFloat4() const;
    /// set matrix44 content
    void SetMatrix44(const Math::matrix44& val);
    /// get matrix44 content
    const Math::matrix44& GetMatrix44() const;
    /// set blob 
    void SetBlob(const Util::Blob& val);
    /// get blob
    const Util::Blob& GetBlob() const;
    /// set guid content
    void SetGuid(const Util::Guid& val);
    /// get guid content
    const Util::Guid& GetGuid() const;
    /// set object pointer
    void SetObject(Core::RefCounted* ptr);
    /// get object pointer
    Core::RefCounted* GetObject() const;
    /// set int array content
    void SetIntArray(const Util::Array<int>& val);
    /// get int array content
    const Util::Array<int>& GetIntArray() const;
    /// set float array content
    void SetFloatArray(const Util::Array<float>& val);
    /// get float array content
    const Util::Array<float>& GetFloatArray() const;
    /// set bool array content
    void SetBoolArray(const Util::Array<bool>& val);
    /// get bool array content
    const Util::Array<bool>& GetBoolArray() const;
    /// set float4 array content
    void SetFloat4Array(const Util::Array<Math::float4>& val);
    /// get float4 array content
    const Util::Array<Math::float4>& GetFloat4Array() const;
    /// set matrix44 array content
    void SetMatrix44Array(const Util::Array<Math::matrix44>& val);
    /// get matrix44 array content
    const Util::Array<Math::matrix44>& GetMatrix44Array() const;
    /// set string array content
    void SetStringArray(const Util::Array<Util::String>& val);
    /// get string array content
    const Util::Array<Util::String>& GetStringArray() const;
    /// set guid array content
    void SetGuidArray(const Util::Array<Util::Guid>& val);
    /// get guid array content
    const Util::Array<Util::Guid>& GetGuidArray() const;
    /// set blob array content
    void SetBlobArray(const Util::Array<Util::Blob>& val);
    /// get blob array content
    const Util::Array<Util::Blob>& GetBlobArray() const;

    /// convert type to string
    static Util::String TypeToString(Type t);
    /// convert string to type
    static Type StringToType(const Util::String& str);

private:
    /// delete current content
    void Delete();
    /// copy current content
    void Copy(const Variant& rhs);

    Type type;
    union
    {
        int i;
        bool b;
        float f[4];
        Math::matrix44* m;
        Util::String* string;
        Util::Guid* guid;
        Util::Blob* blob;
        Core::RefCounted* object;
        Util::Array<int>* intArray;
        Util::Array<float>* floatArray;
        Util::Array<bool>* boolArray;
        Util::Array<Math::float4>* float4Array;
        Util::Array<Math::matrix44>* matrix44Array; 
        Util::Array<Util::String>* stringArray;
        Util::Array<Util::Guid>* guidArray;
        Util::Array<Util::Blob>* blobArray;
    };
};

//------------------------------------------------------------------------------
/**
*/
inline
Variant::Variant() :
    type(Void),
    string(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::Delete()
{
    if (String == this->type)
    {
        n_assert(this->string);
        n_delete(this->string);
        this->string = 0;
    }
    else if (Matrix44 == this->type)
    {
        n_assert(this->m);
        n_delete(this->m);
        this->m = 0;
    }
    else if (Guid == this->type)
    {
        n_assert(this->guid);
        n_delete(this->guid);
        this->guid = 0;
    }
    else if (Blob == this->type)
    {
        n_assert(this->blob);
        n_delete(this->blob);
        this->blob = 0;
    }
    else if (Object == this->type)
    {
        if (this->object)
        {
            this->object->Release();
            this->object = 0;
        }
    }
    else if (IntArray == this->type)
    {
        n_assert(this->intArray);
        n_delete(this->intArray);
        this->intArray = 0;
    }
    else if (FloatArray == this->type)
    {
        n_assert(this->floatArray);
        n_delete(this->floatArray);
        this->floatArray = 0;
    }
    else if (BoolArray == this->type)
    {
        n_assert(this->boolArray);
        n_delete(this->boolArray);
        this->boolArray = 0;
    }
    else if (Float4Array == this->type)
    {
        n_assert(this->float4Array);
        n_delete(this->float4Array);
        this->float4Array = 0;
    }
    else if (Matrix44Array == this->type)
    {
        n_assert(this->matrix44Array);
        n_delete(this->matrix44Array);
        this->matrix44Array = 0;
    }
    else if (StringArray == this->type)
    {
        n_assert(this->stringArray);
        n_delete(this->stringArray);
        this->stringArray = 0;
    }
    else if (GuidArray == this->type)
    {
        n_assert(this->guidArray);
        n_delete(this->guidArray);
        this->guidArray = 0;
    }
    else if (BlobArray == this->type)
    {
        n_assert(this->blobArray);
        n_delete(this->blobArray);
        this->blobArray = 0;
    }
    this->type = Void;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::Clear()
{
    this->Delete();
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::Copy(const Variant& rhs)
{
    n_assert(Void == this->type);
    this->type = rhs.type;
    switch (rhs.type)
    {
        case Void:
            break;
        case Int:
            this->i = rhs.i;
            break;
        case Float:
            this->f[0] = rhs.f[0];
            break;
        case Bool:
            this->b = rhs.b;
            break;
        case Float4:
            this->f[0] = rhs.f[0];
            this->f[1] = rhs.f[1];
            this->f[2] = rhs.f[2];
            this->f[3] = rhs.f[3];
            break;
        case String:
            this->string = n_new(Util::String(*rhs.string));
            break;
        case Matrix44:
            this->m = n_new(Math::matrix44(*rhs.m));
            break;
        case Blob:
            this->blob = n_new(Util::Blob(*rhs.blob));
            break;
        case Guid:
            this->guid = n_new(Util::Guid(*rhs.guid));
            break;
        case Object:
            this->object = rhs.object;
            if (this->object)
            {
                this->object->AddRef();
            }
            break;
        case IntArray:
            this->intArray = n_new(Util::Array<int>(*rhs.intArray));
            break;
        case FloatArray:
            this->floatArray = n_new(Util::Array<float>(*rhs.floatArray));
            break;
        case BoolArray:
            this->boolArray = n_new(Util::Array<bool>(*rhs.boolArray));
            break;
        case Float4Array:
            this->float4Array = n_new(Util::Array<Math::float4>(*rhs.float4Array));
            break;
        case Matrix44Array:
            this->matrix44Array = n_new(Util::Array<Math::matrix44>(*rhs.matrix44Array));
            break;
        case StringArray:
            this->stringArray = n_new(Util::Array<Util::String>(*rhs.stringArray));
            break;
        case GuidArray:
            this->guidArray = n_new(Util::Array<Util::Guid>(*rhs.guidArray));
            break;
        case BlobArray:
            this->blobArray = n_new(Util::Array<Util::Blob>(*rhs.blobArray));
            break;
        default:
            n_error("Variant::Copy(): invalid type!");
            break;
    }
}

//------------------------------------------------------------------------------
/**
*/
inline
Variant::Variant(const Variant& rhs) :
    type(Void)
{
    this->Copy(rhs);
}

//------------------------------------------------------------------------------
/**
*/
inline
Variant::Variant(int rhs) :
    type(Int),
    i(rhs)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
Variant::Variant(float rhs) :
    type(Float)
{
    this->f[0] = rhs;
}

//------------------------------------------------------------------------------
/**
*/
inline
Variant::Variant(bool rhs) :
    type(Bool),
    b(rhs)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
Variant::Variant(const Math::float4& rhs) :
    type(Float4)
{
    rhs.storeu(this->f);
}

//------------------------------------------------------------------------------
/**
*/
inline
Variant::Variant(const Util::String& rhs) :
    type(String)
{
    this->string = n_new(Util::String(rhs));
}

//------------------------------------------------------------------------------
/**
*/
inline
Variant::Variant(const char* chrPtr) :
    type(String)
{
    this->string = n_new(Util::String(chrPtr));
}

//------------------------------------------------------------------------------
/**
*/
inline
Variant::Variant(Core::RefCounted* ptr) :
    type(Object)
{
    this->object = ptr;
    if (this->object)
    {
        this->object->AddRef();
    }
}

//------------------------------------------------------------------------------
/**
*/
inline
Variant::Variant(const Math::matrix44& rhs) :
    type(Matrix44)
{
    this->m = n_new(Math::matrix44(rhs));
}

//------------------------------------------------------------------------------
/**
*/
inline
Variant::Variant(const Util::Blob& rhs) :
    type(Blob)
{
    this->blob = n_new(Util::Blob(rhs));
}

//------------------------------------------------------------------------------
/**
*/
inline
Variant::Variant(const Util::Guid& rhs) :
    type(Guid)
{
    this->guid = n_new(Util::Guid(rhs));
}

//------------------------------------------------------------------------------
/**
*/
inline
Variant::Variant(const Util::Array<int>& rhs) :
    type(IntArray)
{
    this->intArray = n_new(Util::Array<int>(rhs));
}

//------------------------------------------------------------------------------
/**
*/
inline
Variant::Variant(const Util::Array<float>& rhs) :
    type(FloatArray)
{
    this->floatArray = n_new(Util::Array<float>(rhs));
}

//------------------------------------------------------------------------------
/**
*/
inline
Variant::Variant(const Util::Array<bool>& rhs) :
    type(BoolArray)
{
    this->boolArray = n_new(Util::Array<bool>(rhs));
}

//------------------------------------------------------------------------------
/**
*/
inline
Variant::Variant(const Util::Array<Math::float4>& rhs) :
    type(Float4Array)
{
    this->float4Array = n_new(Util::Array<Math::float4>(rhs));
}

//------------------------------------------------------------------------------
/**
*/
inline
Variant::Variant(const Util::Array<Math::matrix44>& rhs) :
    type(Matrix44Array)
{
    this->matrix44Array = n_new(Util::Array<Math::matrix44>(rhs));
}

//------------------------------------------------------------------------------
/**
*/
inline
Variant::Variant(const Util::Array<Util::String>& rhs) :
    type(StringArray)
{
    this->stringArray = n_new(Util::Array<Util::String>(rhs));
}

//------------------------------------------------------------------------------
/**
*/
inline
Variant::Variant(const Util::Array<Util::Guid>& rhs) :
    type(GuidArray)
{
    this->guidArray = n_new(Util::Array<Util::Guid>(rhs));
}

//------------------------------------------------------------------------------
/**
*/
inline
Variant::Variant(const Util::Array<Util::Blob>& rhs) :
    type(BlobArray)
{
    this->blobArray = n_new(Util::Array<Util::Blob>(rhs));
}

//------------------------------------------------------------------------------
/**
*/
inline
Variant::~Variant()
{
    this->Delete();
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::SetType(Type t)
{
    this->Delete();
    this->type = t;
    switch (t)
    {
        case String:
            this->string = n_new(Util::String);
            break;
        case Matrix44:
            this->m = n_new(Math::matrix44);
            break;
        case Blob:
            this->blob = n_new(Util::Blob);
            break;
        case Guid:
            this->guid = n_new(Util::Guid);
            break;
        case Object:
            this->object = 0;
            break;
        case IntArray:
            this->intArray = n_new(Util::Array<int>);
            break;
        case FloatArray:
            this->floatArray = n_new(Util::Array<float>);
            break;
        case BoolArray:
            this->boolArray = n_new(Util::Array<bool>);
            break;
        case Float4Array:
            this->float4Array = n_new(Util::Array<Math::float4>);
            break;
        case Matrix44Array:
            this->matrix44Array = n_new(Util::Array<Math::matrix44>);
            break;
        case StringArray:
            this->stringArray = n_new(Util::Array<Util::String>);
            break;
        case GuidArray:
            this->guidArray = n_new(Util::Array<Util::Guid>);
            break;
        case BlobArray:
            this->blobArray = n_new(Util::Array<Util::Blob>);
            break;
        default:
            break;
    }
}

//------------------------------------------------------------------------------
/**
*/
inline Variant::Type
Variant::GetType() const
{
    return this->type;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::operator=(const Variant& rhs)
{
    this->Delete();
    this->Copy(rhs);
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::operator=(int val)
{
    this->Delete();
    this->type = Int;
    this->i = val;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::operator=(float val)
{
    this->Delete();
    this->type = Float;
    this->f[0] = val;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::operator=(bool val)
{
    this->Delete();
    this->type = Bool;
    this->b = val;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::operator=(const Math::float4& val)
{
    this->Delete();
    this->type = Float4;
    val.storeu(this->f);
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::operator=(const Util::String& s)
{
    if (String == this->type)
    {
        *this->string = s;
    }
    else
    {
        this->Delete();
        this->string = n_new(Util::String(s));
    }
    this->type = String;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::operator=(const char* chrPtr)
{
    *this = Util::String(chrPtr);
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::operator=(const Math::matrix44& val)
{
    if (Matrix44 == this->type)
    {
        *this->m = val;
    }
    else
    {
        this->Delete();
        this->m = n_new(Math::matrix44(val));
    }
    this->type = Matrix44;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::operator=(const Util::Guid& val)
{
    if (Guid == this->type)
    {
        *this->guid = val;
    }
    else
    {
        this->Delete();
        this->guid = n_new(Util::Guid(val));
    }
    this->type = Guid;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::operator=(const Util::Blob& val)
{
    if (Blob == this->type)
    {
        *this->blob = val;
    }
    else
    {
        this->Delete();
        this->blob = n_new(Util::Blob(val));
    }
    this->type = Blob;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::operator=(Core::RefCounted* ptr)
{
    this->Delete();
    this->type = Object;
    this->object = ptr;
    if (this->object)
    {
        this->object->AddRef();
    }
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::operator=(const Util::Array<int>& val)
{
    if (IntArray == this->type)
    {
        *this->intArray = val;
    }
    else
    {
        this->Delete();
        this->intArray = n_new(Util::Array<int>(val));
    }
    this->type = IntArray;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::operator=(const Util::Array<float>& val)
{
    if (FloatArray == this->type)
    {
        *this->floatArray = val;
    }
    else
    {
        this->Delete();
        this->floatArray = n_new(Util::Array<float>(val));
    }
    this->type = FloatArray;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::operator=(const Util::Array<bool>& val)
{
    if (BoolArray == this->type)
    {
        *this->boolArray = val;
    }
    else
    {
        this->Delete();
        this->boolArray = n_new(Util::Array<bool>(val));
    }
    this->type = BoolArray;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::operator=(const Util::Array<Math::float4>& val)
{
    if (Float4Array == this->type)
    {
        *this->float4Array = val;
    }
    else
    {
        this->Delete();
        this->float4Array = n_new(Util::Array<Math::float4>(val));
    }
    this->type = Float4Array;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::operator=(const Util::Array<Math::matrix44>& val)
{
    if (Matrix44Array == this->type)
    {
        *this->matrix44Array = val;
    }
    else
    {
        this->Delete();
        this->matrix44Array = n_new(Util::Array<Math::matrix44>(val));
    }
    this->type = Matrix44Array;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::operator=(const Util::Array<Util::String>& val)
{
    if (StringArray == this->type)
    {
        *this->stringArray = val;
    }
    else
    {
        this->Delete();
        this->stringArray = n_new(Util::Array<Util::String>(val));
    }
    this->type = StringArray;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::operator=(const Util::Array<Util::Guid>& val)
{
    if (GuidArray == this->type)
    {
        *this->guidArray = val;
    }
    else
    {
        this->Delete();
        this->guidArray = n_new(Util::Array<Util::Guid>(val));
    }
    this->type = GuidArray;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::operator=(const Util::Array<Util::Blob>& val)
{
    if (BlobArray == this->type)
    {
        *this->blobArray = val;
    }
    else
    {
        this->Delete();
        this->blobArray = n_new(Util::Array<Util::Blob>(val));
    }
    this->type = BlobArray;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Variant::operator==(const Variant& rhs) const
{
    if (rhs.type == this->type)
    {
        switch (rhs.type)
        {
            case Void:
                return true;
            case Int:
                return (this->i == rhs.i);
            case Bool:
                return (this->b == rhs.b);
            case Float:
                return (this->f[0] == rhs.f[0]);
            case String:
                return ((*this->string) == (*rhs.string));
            case Float4:
                return ((this->f[0] == rhs.f[0]) &&
                        (this->f[1] == rhs.f[1]) &&
                        (this->f[2] == rhs.f[2]) &&
                        (this->f[3] == rhs.f[3]));
            case Guid:
                return ((*this->guid) == (*rhs.guid));
            case Blob:
                return ((*this->blob) == (*rhs.blob));
            case Object:
                return (this->object == rhs.object);
            case Matrix44:
                return this->m == rhs.m;
            default:
                n_error("Variant::operator==(): invalid variant type!");
                return false;
        }
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Variant::operator>(const Variant& rhs) const
{
    if (rhs.type == this->type)
    {
        switch (rhs.type)
        {
        case Void:
            return true;
        case Int:
            return (this->i > rhs.i);
        case Bool:
            return (this->b > rhs.b);
        case Float:
            return (this->f[0] > rhs.f[0]);
        case String:
            return ((*this->string) > (*rhs.string));
        case Float4:
            return ((this->f[0] > rhs.f[0]) &&
                (this->f[1] > rhs.f[1]) &&
                (this->f[2] > rhs.f[2]) &&
                (this->f[3] > rhs.f[3]));
        case Guid:
            return ((*this->guid) > (*rhs.guid));
        case Blob:
            return ((*this->blob) > (*rhs.blob));
        case Object:
            return (this->object > rhs.object);
        default:
            n_error("Variant::operator>(): invalid variant type!");
            return false;
        }
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Variant::operator<(const Variant& rhs) const
{
    if (rhs.type == this->type)
    {
        switch (rhs.type)
        {
        case Void:
            return true;
        case Int:
            return (this->i < rhs.i);
        case Bool:
            return (this->b < rhs.b);
        case Float:
            return (this->f[0] < rhs.f[0]);
        case String:
            return ((*this->string) < (*rhs.string));
        case Float4:
            return ((this->f[0] < rhs.f[0]) &&
                (this->f[1] < rhs.f[1]) &&
                (this->f[2] < rhs.f[2]) &&
                (this->f[3] < rhs.f[3]));
        case Guid:
            return ((*this->guid) < (*rhs.guid));
        case Blob:
            return ((*this->blob) < (*rhs.blob));
        case Object:
            return (this->object < rhs.object);
        default:
            n_error("Variant::operator<(): invalid variant type!");
            return false;
        }
    }
    return false;
}


//------------------------------------------------------------------------------
/**
*/
inline bool
Variant::operator>=(const Variant& rhs) const
{
    if (rhs.type == this->type)
    {
        switch (rhs.type)
        {
        case Void:
            return true;
        case Int:
            return (this->i >= rhs.i);
        case Bool:
            return (this->b >= rhs.b);
        case Float:
            return (this->f[0] >= rhs.f[0]);
        case String:
            return ((*this->string) >= (*rhs.string));
        case Float4:
            return ((this->f[0] >= rhs.f[0]) &&
                (this->f[1] >= rhs.f[1]) &&
                (this->f[2] >= rhs.f[2]) &&
                (this->f[3] >= rhs.f[3]));
        case Guid:
            return ((*this->guid) >= (*rhs.guid));
        case Blob:
            return ((*this->blob) >= (*rhs.blob));
        case Object:
            return (this->object >= rhs.object);
        default:
            n_error("Variant::operator>(): invalid variant type!");
            return false;
        }
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Variant::operator<=(const Variant& rhs) const
{
    if (rhs.type == this->type)
    {
        switch (rhs.type)
        {
        case Void:
            return true;
        case Int:
            return (this->i <= rhs.i);
        case Bool:
            return (this->b <= rhs.b);
        case Float:
            return (this->f[0] <= rhs.f[0]);
        case String:
            return ((*this->string) <= (*rhs.string));
        case Float4:
            return ((this->f[0] <= rhs.f[0]) &&
                (this->f[1] <= rhs.f[1]) &&
                (this->f[2] <= rhs.f[2]) &&
                (this->f[3] <= rhs.f[3]));
        case Guid:
            return ((*this->guid) <= (*rhs.guid));
        case Blob:
            return ((*this->blob) <= (*rhs.blob));
        case Object:
            return (this->object <= rhs.object);
        default:
            n_error("Variant::operator<(): invalid variant type!");
            return false;
        }
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Variant::operator!=(const Variant& rhs) const
{
    return !(*this == rhs);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Variant::operator==(int rhs) const
{
    n_assert(Int == this->type);
    return (this->i == rhs);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Variant::operator==(float rhs) const
{
    n_assert(Float == this->type);
    return (this->f[0] == rhs);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Variant::operator==(bool rhs) const
{
    n_assert(Bool == this->type);
    return (this->b == rhs);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Variant::operator==(const Util::String& rhs) const
{
    n_assert(String == this->type);
    return ((*this->string) == rhs);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Variant::operator==(const char* chrPtr) const
{
    return *this == Util::String(chrPtr);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Variant::operator==(const Math::float4& rhs) const
{
    n_assert(Float4 == this->type);
    return ((this->f[0] == rhs.x()) &&
            (this->f[1] == rhs.y()) &&
            (this->f[2] == rhs.z()) &&
            (this->f[3] == rhs.w()));
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Variant::operator==(const Util::Guid& rhs) const
{
    n_assert(Guid == this->type);
    return (*this->guid) == rhs;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Variant::operator==(Core::RefCounted* ptr) const
{
    n_assert(Object == this->type);
    return this->object == ptr;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Variant::operator!=(int rhs) const
{
    n_assert(Int == this->type);
    return (this->i != rhs);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Variant::operator!=(float rhs) const
{
    n_assert(Float == this->type);
    return (this->f[0] != rhs);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Variant::operator!=(bool rhs) const
{
    n_assert(Bool == this->type);
    return (this->b != rhs);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Variant::operator!=(const Util::String& rhs) const
{
    n_assert(String == this->type);
    return (*this->string) != rhs;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Variant::operator!=(const char* chrPtr) const
{
    return *this != Util::String(chrPtr);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Variant::operator!=(const Math::float4& rhs) const
{
    n_assert(Float4 == this->type);
    return ((this->f[0] != rhs.x()) ||
            (this->f[1] != rhs.y()) ||
            (this->f[2] != rhs.z()) ||
            (this->f[3] != rhs.w()));
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Variant::operator!=(const Util::Guid& rhs) const
{
    n_assert(Guid == this->type);
    return (*this->guid) != rhs;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Variant::operator!=(Core::RefCounted* ptr) const
{
    n_assert(Object == this->type);
    return (this->object == ptr);
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::SetInt(int val)
{
    *this = val;
}

//------------------------------------------------------------------------------
/**
*/
inline int
Variant::GetInt() const
{
    n_assert(Int == this->type);
    return this->i;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::SetFloat(float val)
{
    *this = val;
}

//------------------------------------------------------------------------------
/**
*/
inline float
Variant::GetFloat() const
{
    n_assert(Float == this->type);
    return this->f[0];
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::SetBool(bool val)
{
    *this = val;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Variant::GetBool() const
{
    n_assert(Bool == this->type);
    return this->b;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::SetString(const Util::String& val)
{
    *this = val;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::String&
Variant::GetString() const
{
    n_assert(String == this->type);
    return *(this->string);
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::SetFloat4(const Math::float4& val)
{
    *this = val;
}

//------------------------------------------------------------------------------
/**
*/
inline Math::float4
Variant::GetFloat4() const
{
    n_assert(Float4 == this->type);
    return Math::float4(this->f[0], this->f[1], this->f[2], this->f[3]);
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::SetMatrix44(const Math::matrix44& val)
{
    *this = val;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::matrix44&
Variant::GetMatrix44() const
{
    n_assert(Matrix44 == this->type);
    return *(this->m);
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::SetGuid(const Util::Guid& val)
{
    *this = val;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Guid&
Variant::GetGuid() const
{
    n_assert(Guid == this->type);
    return *(this->guid);
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::SetBlob(const Util::Blob& val)
{
    *this = val;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Blob&
Variant::GetBlob() const
{
    n_assert(Blob == this->type);
    return *(this->blob);
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::SetObject(Core::RefCounted* ptr)
{
    *this = ptr;
}

//------------------------------------------------------------------------------
/**
*/
inline Core::RefCounted*
Variant::GetObject() const
{
    n_assert(Object == this->type);
    return this->object;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::SetIntArray(const Util::Array<int>& val)
{
    *this = val;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<int>&
Variant::GetIntArray() const
{
    n_assert(IntArray == this->type);
    return *(this->intArray);
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::SetFloatArray(const Util::Array<float>& val)
{
    *this = val;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<float>&
Variant::GetFloatArray() const
{
    n_assert(FloatArray == this->type);
    return *(this->floatArray);
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::SetBoolArray(const Util::Array<bool>& val)
{
    *this = val;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<bool>&
Variant::GetBoolArray() const
{
    n_assert(BoolArray == this->type);
    return *(this->boolArray);
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::SetFloat4Array(const Util::Array<Math::float4>& val)
{
    *this = val;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<Math::float4>&
Variant::GetFloat4Array() const
{
    n_assert(Float4Array == this->type);
    return *(this->float4Array);
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::SetMatrix44Array(const Util::Array<Math::matrix44>& val)
{
    *this = val;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<Math::matrix44>&
Variant::GetMatrix44Array() const
{
    n_assert(Matrix44Array == this->type);
    return *(this->matrix44Array);
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::SetStringArray(const Util::Array<Util::String>& val)
{
    *this = val;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<Util::String>&
Variant::GetStringArray() const
{
    n_assert(StringArray == this->type);
    return *(this->stringArray);
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::SetGuidArray(const Util::Array<Util::Guid>& val)
{
    *this = val;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<Util::Guid>&
Variant::GetGuidArray() const
{
    n_assert(GuidArray == this->type);
    return *(this->guidArray);
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::SetBlobArray(const Util::Array<Util::Blob>& val)
{
    *this = val;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<Util::Blob>&
Variant::GetBlobArray() const
{
    n_assert(BlobArray == this->type);
    return *(this->blobArray);
}

//------------------------------------------------------------------------------
/**
*/
inline Util::String
Variant::TypeToString(Type t)
{
    switch (t)
    {
        case Void:          return "void";
        case Int:           return "int";
        case Float:         return "float";
        case Bool:          return "bool";
        case Float4:        return "float4";
        case String:        return "string";
        case Matrix44:      return "matrix44";
        case Blob:          return "blob";
        case Guid:          return "guid";
        case Object:        return "object";
        case IntArray:      return "intarray";
        case FloatArray:    return "floatarray";
        case BoolArray:     return "boolarray";
        case Float4Array:   return "float4array";
        case Matrix44Array: return "matrix44array";
        case StringArray:   return "stringarray";
        case GuidArray:     return "guidarray";
        case BlobArray:     return "blobarray";
        default:
            n_error("Variant::TypeToString(): invalid type enum '%d'!", t);
            return 0;
    }
}

//------------------------------------------------------------------------------
/**
*/
inline Variant::Type
Variant::StringToType(const Util::String& str)
{
    if      ("void" == str)             return Void;
    else if ("int" == str)              return Int;
    else if ("float" == str)            return Float;
    else if ("bool" == str)             return Bool;
    else if ("float4" == str)           return Float4;
    else if ("color" == str)            return Float4; // NOT A BUG!
    else if ("string" == str)           return String;
    else if ("matrix44" == str)         return Matrix44;
    else if ("blob" == str)             return Blob;
    else if ("guid" == str)             return Guid;
    else if ("object" == str)           return Object;
    else if ("intarray" == str)         return IntArray;
    else if ("floatarray" == str)       return FloatArray;
    else if ("boolarray" == str)        return BoolArray;
    else if ("float4array" == str)      return Float4Array;
    else if ("matrix44array" == str)    return Matrix44Array;
    else if ("stringarray" == str)      return StringArray;
    else if ("guidarray" == str)        return GuidArray;
    else if ("blobarray" == str)        return BlobArray;
    else
    {
        n_error("Variant::StringToType(): invalid type string '%s'!", str.AsCharPtr());
        return Void;
    }
}

} // namespace Util
//------------------------------------------------------------------------------
