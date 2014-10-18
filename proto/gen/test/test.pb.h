// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: test/test.proto

#ifndef PROTOBUF_test_2ftest_2eproto__INCLUDED
#define PROTOBUF_test_2ftest_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2006000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2006000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace test {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_test_2ftest_2eproto();
void protobuf_AssignDesc_test_2ftest_2eproto();
void protobuf_ShutdownFile_test_2ftest_2eproto();

class Cache;
class Cache__PrimaryKey;
class Cache__MaxLength;

// ===================================================================

class Cache__PrimaryKey : public ::google::protobuf::Message {
 public:
  Cache__PrimaryKey();
  virtual ~Cache__PrimaryKey();

  Cache__PrimaryKey(const Cache__PrimaryKey& from);

  inline Cache__PrimaryKey& operator=(const Cache__PrimaryKey& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const Cache__PrimaryKey& default_instance();

  void Swap(Cache__PrimaryKey* other);

  // implements Message ----------------------------------------------

  Cache__PrimaryKey* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Cache__PrimaryKey& from);
  void MergeFrom(const Cache__PrimaryKey& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional string name = 1;
  inline bool has_name() const;
  inline void clear_name();
  static const int kNameFieldNumber = 1;
  inline const ::std::string& name() const;
  inline void set_name(const ::std::string& value);
  inline void set_name(const char* value);
  inline void set_name(const char* value, size_t size);
  inline ::std::string* mutable_name();
  inline ::std::string* release_name();
  inline void set_allocated_name(::std::string* name);

  // optional string area = 2;
  inline bool has_area() const;
  inline void clear_area();
  static const int kAreaFieldNumber = 2;
  inline const ::std::string& area() const;
  inline void set_area(const ::std::string& value);
  inline void set_area(const char* value);
  inline void set_area(const char* value, size_t size);
  inline ::std::string* mutable_area();
  inline ::std::string* release_area();
  inline void set_allocated_area(::std::string* area);

  // @@protoc_insertion_point(class_scope:test.Cache._PrimaryKey)
 private:
  inline void set_has_name();
  inline void clear_has_name();
  inline void set_has_area();
  inline void clear_has_area();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::std::string* name_;
  ::std::string* area_;
  friend void  protobuf_AddDesc_test_2ftest_2eproto();
  friend void protobuf_AssignDesc_test_2ftest_2eproto();
  friend void protobuf_ShutdownFile_test_2ftest_2eproto();

  void InitAsDefaultInstance();
  static Cache__PrimaryKey* default_instance_;
};
// -------------------------------------------------------------------

class Cache__MaxLength : public ::google::protobuf::Message {
 public:
  Cache__MaxLength();
  virtual ~Cache__MaxLength();

  Cache__MaxLength(const Cache__MaxLength& from);

  inline Cache__MaxLength& operator=(const Cache__MaxLength& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const Cache__MaxLength& default_instance();

  void Swap(Cache__MaxLength* other);

  // implements Message ----------------------------------------------

  Cache__MaxLength* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Cache__MaxLength& from);
  void MergeFrom(const Cache__MaxLength& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional uint32 name = 1 [default = 32];
  inline bool has_name() const;
  inline void clear_name();
  static const int kNameFieldNumber = 1;
  inline ::google::protobuf::uint32 name() const;
  inline void set_name(::google::protobuf::uint32 value);

  // optional uint32 area = 2 [default = 5];
  inline bool has_area() const;
  inline void clear_area();
  static const int kAreaFieldNumber = 2;
  inline ::google::protobuf::uint32 area() const;
  inline void set_area(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:test.Cache._MaxLength)
 private:
  inline void set_has_name();
  inline void clear_has_name();
  inline void set_has_area();
  inline void clear_has_area();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::google::protobuf::uint32 name_;
  ::google::protobuf::uint32 area_;
  friend void  protobuf_AddDesc_test_2ftest_2eproto();
  friend void protobuf_AssignDesc_test_2ftest_2eproto();
  friend void protobuf_ShutdownFile_test_2ftest_2eproto();

  void InitAsDefaultInstance();
  static Cache__MaxLength* default_instance_;
};
// -------------------------------------------------------------------

class Cache : public ::google::protobuf::Message {
 public:
  Cache();
  virtual ~Cache();

  Cache(const Cache& from);

  inline Cache& operator=(const Cache& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const Cache& default_instance();

  void Swap(Cache* other);

  // implements Message ----------------------------------------------

  Cache* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Cache& from);
  void MergeFrom(const Cache& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  typedef Cache__PrimaryKey _PrimaryKey;
  typedef Cache__MaxLength _MaxLength;

  // accessors -------------------------------------------------------

  // required string name = 1;
  inline bool has_name() const;
  inline void clear_name();
  static const int kNameFieldNumber = 1;
  inline const ::std::string& name() const;
  inline void set_name(const ::std::string& value);
  inline void set_name(const char* value);
  inline void set_name(const char* value, size_t size);
  inline ::std::string* mutable_name();
  inline ::std::string* release_name();
  inline void set_allocated_name(::std::string* name);

  // required uint32 area = 2;
  inline bool has_area() const;
  inline void clear_area();
  static const int kAreaFieldNumber = 2;
  inline ::google::protobuf::uint32 area() const;
  inline void set_area(::google::protobuf::uint32 value);

  // required uint64 gid = 3;
  inline bool has_gid() const;
  inline void clear_gid();
  static const int kGidFieldNumber = 3;
  inline ::google::protobuf::uint64 gid() const;
  inline void set_gid(::google::protobuf::uint64 value);

  // @@protoc_insertion_point(class_scope:test.Cache)
 private:
  inline void set_has_name();
  inline void clear_has_name();
  inline void set_has_area();
  inline void clear_has_area();
  inline void set_has_gid();
  inline void clear_has_gid();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::std::string* name_;
  ::google::protobuf::uint64 gid_;
  ::google::protobuf::uint32 area_;
  friend void  protobuf_AddDesc_test_2ftest_2eproto();
  friend void protobuf_AssignDesc_test_2ftest_2eproto();
  friend void protobuf_ShutdownFile_test_2ftest_2eproto();

  void InitAsDefaultInstance();
  static Cache* default_instance_;
};
// ===================================================================


// ===================================================================

// Cache__PrimaryKey

// optional string name = 1;
inline bool Cache__PrimaryKey::has_name() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Cache__PrimaryKey::set_has_name() {
  _has_bits_[0] |= 0x00000001u;
}
inline void Cache__PrimaryKey::clear_has_name() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void Cache__PrimaryKey::clear_name() {
  if (name_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    name_->clear();
  }
  clear_has_name();
}
inline const ::std::string& Cache__PrimaryKey::name() const {
  // @@protoc_insertion_point(field_get:test.Cache._PrimaryKey.name)
  return *name_;
}
inline void Cache__PrimaryKey::set_name(const ::std::string& value) {
  set_has_name();
  if (name_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    name_ = new ::std::string;
  }
  name_->assign(value);
  // @@protoc_insertion_point(field_set:test.Cache._PrimaryKey.name)
}
inline void Cache__PrimaryKey::set_name(const char* value) {
  set_has_name();
  if (name_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    name_ = new ::std::string;
  }
  name_->assign(value);
  // @@protoc_insertion_point(field_set_char:test.Cache._PrimaryKey.name)
}
inline void Cache__PrimaryKey::set_name(const char* value, size_t size) {
  set_has_name();
  if (name_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    name_ = new ::std::string;
  }
  name_->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:test.Cache._PrimaryKey.name)
}
inline ::std::string* Cache__PrimaryKey::mutable_name() {
  set_has_name();
  if (name_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    name_ = new ::std::string;
  }
  // @@protoc_insertion_point(field_mutable:test.Cache._PrimaryKey.name)
  return name_;
}
inline ::std::string* Cache__PrimaryKey::release_name() {
  clear_has_name();
  if (name_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    return NULL;
  } else {
    ::std::string* temp = name_;
    name_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    return temp;
  }
}
inline void Cache__PrimaryKey::set_allocated_name(::std::string* name) {
  if (name_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete name_;
  }
  if (name) {
    set_has_name();
    name_ = name;
  } else {
    clear_has_name();
    name_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  // @@protoc_insertion_point(field_set_allocated:test.Cache._PrimaryKey.name)
}

// optional string area = 2;
inline bool Cache__PrimaryKey::has_area() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void Cache__PrimaryKey::set_has_area() {
  _has_bits_[0] |= 0x00000002u;
}
inline void Cache__PrimaryKey::clear_has_area() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void Cache__PrimaryKey::clear_area() {
  if (area_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    area_->clear();
  }
  clear_has_area();
}
inline const ::std::string& Cache__PrimaryKey::area() const {
  // @@protoc_insertion_point(field_get:test.Cache._PrimaryKey.area)
  return *area_;
}
inline void Cache__PrimaryKey::set_area(const ::std::string& value) {
  set_has_area();
  if (area_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    area_ = new ::std::string;
  }
  area_->assign(value);
  // @@protoc_insertion_point(field_set:test.Cache._PrimaryKey.area)
}
inline void Cache__PrimaryKey::set_area(const char* value) {
  set_has_area();
  if (area_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    area_ = new ::std::string;
  }
  area_->assign(value);
  // @@protoc_insertion_point(field_set_char:test.Cache._PrimaryKey.area)
}
inline void Cache__PrimaryKey::set_area(const char* value, size_t size) {
  set_has_area();
  if (area_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    area_ = new ::std::string;
  }
  area_->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:test.Cache._PrimaryKey.area)
}
inline ::std::string* Cache__PrimaryKey::mutable_area() {
  set_has_area();
  if (area_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    area_ = new ::std::string;
  }
  // @@protoc_insertion_point(field_mutable:test.Cache._PrimaryKey.area)
  return area_;
}
inline ::std::string* Cache__PrimaryKey::release_area() {
  clear_has_area();
  if (area_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    return NULL;
  } else {
    ::std::string* temp = area_;
    area_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    return temp;
  }
}
inline void Cache__PrimaryKey::set_allocated_area(::std::string* area) {
  if (area_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete area_;
  }
  if (area) {
    set_has_area();
    area_ = area;
  } else {
    clear_has_area();
    area_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  // @@protoc_insertion_point(field_set_allocated:test.Cache._PrimaryKey.area)
}

// -------------------------------------------------------------------

// Cache__MaxLength

// optional uint32 name = 1 [default = 32];
inline bool Cache__MaxLength::has_name() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Cache__MaxLength::set_has_name() {
  _has_bits_[0] |= 0x00000001u;
}
inline void Cache__MaxLength::clear_has_name() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void Cache__MaxLength::clear_name() {
  name_ = 32u;
  clear_has_name();
}
inline ::google::protobuf::uint32 Cache__MaxLength::name() const {
  // @@protoc_insertion_point(field_get:test.Cache._MaxLength.name)
  return name_;
}
inline void Cache__MaxLength::set_name(::google::protobuf::uint32 value) {
  set_has_name();
  name_ = value;
  // @@protoc_insertion_point(field_set:test.Cache._MaxLength.name)
}

// optional uint32 area = 2 [default = 5];
inline bool Cache__MaxLength::has_area() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void Cache__MaxLength::set_has_area() {
  _has_bits_[0] |= 0x00000002u;
}
inline void Cache__MaxLength::clear_has_area() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void Cache__MaxLength::clear_area() {
  area_ = 5u;
  clear_has_area();
}
inline ::google::protobuf::uint32 Cache__MaxLength::area() const {
  // @@protoc_insertion_point(field_get:test.Cache._MaxLength.area)
  return area_;
}
inline void Cache__MaxLength::set_area(::google::protobuf::uint32 value) {
  set_has_area();
  area_ = value;
  // @@protoc_insertion_point(field_set:test.Cache._MaxLength.area)
}

// -------------------------------------------------------------------

// Cache

// required string name = 1;
inline bool Cache::has_name() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Cache::set_has_name() {
  _has_bits_[0] |= 0x00000001u;
}
inline void Cache::clear_has_name() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void Cache::clear_name() {
  if (name_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    name_->clear();
  }
  clear_has_name();
}
inline const ::std::string& Cache::name() const {
  // @@protoc_insertion_point(field_get:test.Cache.name)
  return *name_;
}
inline void Cache::set_name(const ::std::string& value) {
  set_has_name();
  if (name_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    name_ = new ::std::string;
  }
  name_->assign(value);
  // @@protoc_insertion_point(field_set:test.Cache.name)
}
inline void Cache::set_name(const char* value) {
  set_has_name();
  if (name_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    name_ = new ::std::string;
  }
  name_->assign(value);
  // @@protoc_insertion_point(field_set_char:test.Cache.name)
}
inline void Cache::set_name(const char* value, size_t size) {
  set_has_name();
  if (name_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    name_ = new ::std::string;
  }
  name_->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:test.Cache.name)
}
inline ::std::string* Cache::mutable_name() {
  set_has_name();
  if (name_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    name_ = new ::std::string;
  }
  // @@protoc_insertion_point(field_mutable:test.Cache.name)
  return name_;
}
inline ::std::string* Cache::release_name() {
  clear_has_name();
  if (name_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    return NULL;
  } else {
    ::std::string* temp = name_;
    name_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    return temp;
  }
}
inline void Cache::set_allocated_name(::std::string* name) {
  if (name_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete name_;
  }
  if (name) {
    set_has_name();
    name_ = name;
  } else {
    clear_has_name();
    name_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  // @@protoc_insertion_point(field_set_allocated:test.Cache.name)
}

// required uint32 area = 2;
inline bool Cache::has_area() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void Cache::set_has_area() {
  _has_bits_[0] |= 0x00000002u;
}
inline void Cache::clear_has_area() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void Cache::clear_area() {
  area_ = 0u;
  clear_has_area();
}
inline ::google::protobuf::uint32 Cache::area() const {
  // @@protoc_insertion_point(field_get:test.Cache.area)
  return area_;
}
inline void Cache::set_area(::google::protobuf::uint32 value) {
  set_has_area();
  area_ = value;
  // @@protoc_insertion_point(field_set:test.Cache.area)
}

// required uint64 gid = 3;
inline bool Cache::has_gid() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void Cache::set_has_gid() {
  _has_bits_[0] |= 0x00000004u;
}
inline void Cache::clear_has_gid() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void Cache::clear_gid() {
  gid_ = GOOGLE_ULONGLONG(0);
  clear_has_gid();
}
inline ::google::protobuf::uint64 Cache::gid() const {
  // @@protoc_insertion_point(field_get:test.Cache.gid)
  return gid_;
}
inline void Cache::set_gid(::google::protobuf::uint64 value) {
  set_has_gid();
  gid_ = value;
  // @@protoc_insertion_point(field_set:test.Cache.gid)
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace test

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_test_2ftest_2eproto__INCLUDED
