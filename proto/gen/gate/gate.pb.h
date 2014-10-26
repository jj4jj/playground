// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: gate/gate.proto

#ifndef PROTOBUF_gate_2fgate_2eproto__INCLUDED
#define PROTOBUF_gate_2fgate_2eproto__INCLUDED

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

namespace gate {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_gate_2fgate_2eproto();
void protobuf_AssignDesc_gate_2fgate_2eproto();
void protobuf_ShutdownFile_gate_2fgate_2eproto();

class Dummy;

// ===================================================================

class Dummy : public ::google::protobuf::Message {
 public:
  Dummy();
  virtual ~Dummy();

  Dummy(const Dummy& from);

  inline Dummy& operator=(const Dummy& from) {
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
  static const Dummy& default_instance();

  void Swap(Dummy* other);

  // implements Message ----------------------------------------------

  Dummy* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Dummy& from);
  void MergeFrom(const Dummy& from);
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

  // optional uint32 dummy = 1;
  inline bool has_dummy() const;
  inline void clear_dummy();
  static const int kDummyFieldNumber = 1;
  inline ::google::protobuf::uint32 dummy() const;
  inline void set_dummy(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:gate.Dummy)
 private:
  inline void set_has_dummy();
  inline void clear_has_dummy();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::google::protobuf::uint32 dummy_;
  friend void  protobuf_AddDesc_gate_2fgate_2eproto();
  friend void protobuf_AssignDesc_gate_2fgate_2eproto();
  friend void protobuf_ShutdownFile_gate_2fgate_2eproto();

  void InitAsDefaultInstance();
  static Dummy* default_instance_;
};
// ===================================================================


// ===================================================================

// Dummy

// optional uint32 dummy = 1;
inline bool Dummy::has_dummy() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Dummy::set_has_dummy() {
  _has_bits_[0] |= 0x00000001u;
}
inline void Dummy::clear_has_dummy() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void Dummy::clear_dummy() {
  dummy_ = 0u;
  clear_has_dummy();
}
inline ::google::protobuf::uint32 Dummy::dummy() const {
  // @@protoc_insertion_point(field_get:gate.Dummy.dummy)
  return dummy_;
}
inline void Dummy::set_dummy(::google::protobuf::uint32 value) {
  set_has_dummy();
  dummy_ = value;
  // @@protoc_insertion_point(field_set:gate.Dummy.dummy)
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace gate

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_gate_2fgate_2eproto__INCLUDED
