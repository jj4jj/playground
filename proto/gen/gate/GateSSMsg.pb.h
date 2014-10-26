// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: gate/GateSSMsg.proto

#ifndef PROTOBUF_gate_2fGateSSMsg_2eproto__INCLUDED
#define PROTOBUF_gate_2fGateSSMsg_2eproto__INCLUDED

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
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace gate {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_gate_2fGateSSMsg_2eproto();
void protobuf_AssignDesc_gate_2fGateSSMsg_2eproto();
void protobuf_ShutdownFile_gate_2fGateSSMsg_2eproto();

class GateSSMsg;

enum GateSSMsg_EventType {
  GateSSMsg_EventType_EVENT_CONNECTED = 1,
  GateSSMsg_EventType_EVENT_CLOSE = 2,
  GateSSMsg_EventType_EVENT_DATA = 3
};
bool GateSSMsg_EventType_IsValid(int value);
const GateSSMsg_EventType GateSSMsg_EventType_EventType_MIN = GateSSMsg_EventType_EVENT_CONNECTED;
const GateSSMsg_EventType GateSSMsg_EventType_EventType_MAX = GateSSMsg_EventType_EVENT_DATA;
const int GateSSMsg_EventType_EventType_ARRAYSIZE = GateSSMsg_EventType_EventType_MAX + 1;

const ::google::protobuf::EnumDescriptor* GateSSMsg_EventType_descriptor();
inline const ::std::string& GateSSMsg_EventType_Name(GateSSMsg_EventType value) {
  return ::google::protobuf::internal::NameOfEnum(
    GateSSMsg_EventType_descriptor(), value);
}
inline bool GateSSMsg_EventType_Parse(
    const ::std::string& name, GateSSMsg_EventType* value) {
  return ::google::protobuf::internal::ParseNamedEnum<GateSSMsg_EventType>(
    GateSSMsg_EventType_descriptor(), name, value);
}
enum GateSSMsg_ConnectionCloseReason {
  GateSSMsg_ConnectionCloseReason_CONNECTION_CLOSE_BY_DEFAULT = 0,
  GateSSMsg_ConnectionCloseReason_CONNECTION_CLOSE_BY_CLIENT = 1,
  GateSSMsg_ConnectionCloseReason_CONNECTION_CLOSE_STOP_SVR = 2,
  GateSSMsg_ConnectionCloseReason_CONNECTION_CLOSE_EXCEPTION = 3,
  GateSSMsg_ConnectionCloseReason_CONNECTION_CLOSE_BY_SERVER = 4
};
bool GateSSMsg_ConnectionCloseReason_IsValid(int value);
const GateSSMsg_ConnectionCloseReason GateSSMsg_ConnectionCloseReason_ConnectionCloseReason_MIN = GateSSMsg_ConnectionCloseReason_CONNECTION_CLOSE_BY_DEFAULT;
const GateSSMsg_ConnectionCloseReason GateSSMsg_ConnectionCloseReason_ConnectionCloseReason_MAX = GateSSMsg_ConnectionCloseReason_CONNECTION_CLOSE_BY_SERVER;
const int GateSSMsg_ConnectionCloseReason_ConnectionCloseReason_ARRAYSIZE = GateSSMsg_ConnectionCloseReason_ConnectionCloseReason_MAX + 1;

const ::google::protobuf::EnumDescriptor* GateSSMsg_ConnectionCloseReason_descriptor();
inline const ::std::string& GateSSMsg_ConnectionCloseReason_Name(GateSSMsg_ConnectionCloseReason value) {
  return ::google::protobuf::internal::NameOfEnum(
    GateSSMsg_ConnectionCloseReason_descriptor(), value);
}
inline bool GateSSMsg_ConnectionCloseReason_Parse(
    const ::std::string& name, GateSSMsg_ConnectionCloseReason* value) {
  return ::google::protobuf::internal::ParseNamedEnum<GateSSMsg_ConnectionCloseReason>(
    GateSSMsg_ConnectionCloseReason_descriptor(), name, value);
}
// ===================================================================

class GateSSMsg : public ::google::protobuf::Message {
 public:
  GateSSMsg();
  virtual ~GateSSMsg();

  GateSSMsg(const GateSSMsg& from);

  inline GateSSMsg& operator=(const GateSSMsg& from) {
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
  static const GateSSMsg& default_instance();

  void Swap(GateSSMsg* other);

  // implements Message ----------------------------------------------

  GateSSMsg* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const GateSSMsg& from);
  void MergeFrom(const GateSSMsg& from);
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

  typedef GateSSMsg_EventType EventType;
  static const EventType EVENT_CONNECTED = GateSSMsg_EventType_EVENT_CONNECTED;
  static const EventType EVENT_CLOSE = GateSSMsg_EventType_EVENT_CLOSE;
  static const EventType EVENT_DATA = GateSSMsg_EventType_EVENT_DATA;
  static inline bool EventType_IsValid(int value) {
    return GateSSMsg_EventType_IsValid(value);
  }
  static const EventType EventType_MIN =
    GateSSMsg_EventType_EventType_MIN;
  static const EventType EventType_MAX =
    GateSSMsg_EventType_EventType_MAX;
  static const int EventType_ARRAYSIZE =
    GateSSMsg_EventType_EventType_ARRAYSIZE;
  static inline const ::google::protobuf::EnumDescriptor*
  EventType_descriptor() {
    return GateSSMsg_EventType_descriptor();
  }
  static inline const ::std::string& EventType_Name(EventType value) {
    return GateSSMsg_EventType_Name(value);
  }
  static inline bool EventType_Parse(const ::std::string& name,
      EventType* value) {
    return GateSSMsg_EventType_Parse(name, value);
  }

  typedef GateSSMsg_ConnectionCloseReason ConnectionCloseReason;
  static const ConnectionCloseReason CONNECTION_CLOSE_BY_DEFAULT = GateSSMsg_ConnectionCloseReason_CONNECTION_CLOSE_BY_DEFAULT;
  static const ConnectionCloseReason CONNECTION_CLOSE_BY_CLIENT = GateSSMsg_ConnectionCloseReason_CONNECTION_CLOSE_BY_CLIENT;
  static const ConnectionCloseReason CONNECTION_CLOSE_STOP_SVR = GateSSMsg_ConnectionCloseReason_CONNECTION_CLOSE_STOP_SVR;
  static const ConnectionCloseReason CONNECTION_CLOSE_EXCEPTION = GateSSMsg_ConnectionCloseReason_CONNECTION_CLOSE_EXCEPTION;
  static const ConnectionCloseReason CONNECTION_CLOSE_BY_SERVER = GateSSMsg_ConnectionCloseReason_CONNECTION_CLOSE_BY_SERVER;
  static inline bool ConnectionCloseReason_IsValid(int value) {
    return GateSSMsg_ConnectionCloseReason_IsValid(value);
  }
  static const ConnectionCloseReason ConnectionCloseReason_MIN =
    GateSSMsg_ConnectionCloseReason_ConnectionCloseReason_MIN;
  static const ConnectionCloseReason ConnectionCloseReason_MAX =
    GateSSMsg_ConnectionCloseReason_ConnectionCloseReason_MAX;
  static const int ConnectionCloseReason_ARRAYSIZE =
    GateSSMsg_ConnectionCloseReason_ConnectionCloseReason_ARRAYSIZE;
  static inline const ::google::protobuf::EnumDescriptor*
  ConnectionCloseReason_descriptor() {
    return GateSSMsg_ConnectionCloseReason_descriptor();
  }
  static inline const ::std::string& ConnectionCloseReason_Name(ConnectionCloseReason value) {
    return GateSSMsg_ConnectionCloseReason_Name(value);
  }
  static inline bool ConnectionCloseReason_Parse(const ::std::string& name,
      ConnectionCloseReason* value) {
    return GateSSMsg_ConnectionCloseReason_Parse(name, value);
  }

  // accessors -------------------------------------------------------

  // required .gate.GateSSMsg.EventType event = 1;
  inline bool has_event() const;
  inline void clear_event();
  static const int kEventFieldNumber = 1;
  inline ::gate::GateSSMsg_EventType event() const;
  inline void set_event(::gate::GateSSMsg_EventType value);

  // required uint32 idx = 2;
  inline bool has_idx() const;
  inline void clear_idx();
  static const int kIdxFieldNumber = 2;
  inline ::google::protobuf::uint32 idx() const;
  inline void set_idx(::google::protobuf::uint32 value);

  // required uint64 uid = 3;
  inline bool has_uid() const;
  inline void clear_uid();
  static const int kUidFieldNumber = 3;
  inline ::google::protobuf::uint64 uid() const;
  inline void set_uid(::google::protobuf::uint64 value);

  // required uint32 area = 4;
  inline bool has_area() const;
  inline void clear_area();
  static const int kAreaFieldNumber = 4;
  inline ::google::protobuf::uint32 area() const;
  inline void set_area(::google::protobuf::uint32 value);

  // optional uint32 ip = 20;
  inline bool has_ip() const;
  inline void clear_ip();
  static const int kIpFieldNumber = 20;
  inline ::google::protobuf::uint32 ip() const;
  inline void set_ip(::google::protobuf::uint32 value);

  // optional uint32 port = 21;
  inline bool has_port() const;
  inline void clear_port();
  static const int kPortFieldNumber = 21;
  inline ::google::protobuf::uint32 port() const;
  inline void set_port(::google::protobuf::uint32 value);

  // optional uint32 reason = 22;
  inline bool has_reason() const;
  inline void clear_reason();
  static const int kReasonFieldNumber = 22;
  inline ::google::protobuf::uint32 reason() const;
  inline void set_reason(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:gate.GateSSMsg)
 private:
  inline void set_has_event();
  inline void clear_has_event();
  inline void set_has_idx();
  inline void clear_has_idx();
  inline void set_has_uid();
  inline void clear_has_uid();
  inline void set_has_area();
  inline void clear_has_area();
  inline void set_has_ip();
  inline void clear_has_ip();
  inline void set_has_port();
  inline void clear_has_port();
  inline void set_has_reason();
  inline void clear_has_reason();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  int event_;
  ::google::protobuf::uint32 idx_;
  ::google::protobuf::uint64 uid_;
  ::google::protobuf::uint32 area_;
  ::google::protobuf::uint32 ip_;
  ::google::protobuf::uint32 port_;
  ::google::protobuf::uint32 reason_;
  friend void  protobuf_AddDesc_gate_2fGateSSMsg_2eproto();
  friend void protobuf_AssignDesc_gate_2fGateSSMsg_2eproto();
  friend void protobuf_ShutdownFile_gate_2fGateSSMsg_2eproto();

  void InitAsDefaultInstance();
  static GateSSMsg* default_instance_;
};
// ===================================================================


// ===================================================================

// GateSSMsg

// required .gate.GateSSMsg.EventType event = 1;
inline bool GateSSMsg::has_event() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void GateSSMsg::set_has_event() {
  _has_bits_[0] |= 0x00000001u;
}
inline void GateSSMsg::clear_has_event() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void GateSSMsg::clear_event() {
  event_ = 1;
  clear_has_event();
}
inline ::gate::GateSSMsg_EventType GateSSMsg::event() const {
  // @@protoc_insertion_point(field_get:gate.GateSSMsg.event)
  return static_cast< ::gate::GateSSMsg_EventType >(event_);
}
inline void GateSSMsg::set_event(::gate::GateSSMsg_EventType value) {
  assert(::gate::GateSSMsg_EventType_IsValid(value));
  set_has_event();
  event_ = value;
  // @@protoc_insertion_point(field_set:gate.GateSSMsg.event)
}

// required uint32 idx = 2;
inline bool GateSSMsg::has_idx() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void GateSSMsg::set_has_idx() {
  _has_bits_[0] |= 0x00000002u;
}
inline void GateSSMsg::clear_has_idx() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void GateSSMsg::clear_idx() {
  idx_ = 0u;
  clear_has_idx();
}
inline ::google::protobuf::uint32 GateSSMsg::idx() const {
  // @@protoc_insertion_point(field_get:gate.GateSSMsg.idx)
  return idx_;
}
inline void GateSSMsg::set_idx(::google::protobuf::uint32 value) {
  set_has_idx();
  idx_ = value;
  // @@protoc_insertion_point(field_set:gate.GateSSMsg.idx)
}

// required uint64 uid = 3;
inline bool GateSSMsg::has_uid() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void GateSSMsg::set_has_uid() {
  _has_bits_[0] |= 0x00000004u;
}
inline void GateSSMsg::clear_has_uid() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void GateSSMsg::clear_uid() {
  uid_ = GOOGLE_ULONGLONG(0);
  clear_has_uid();
}
inline ::google::protobuf::uint64 GateSSMsg::uid() const {
  // @@protoc_insertion_point(field_get:gate.GateSSMsg.uid)
  return uid_;
}
inline void GateSSMsg::set_uid(::google::protobuf::uint64 value) {
  set_has_uid();
  uid_ = value;
  // @@protoc_insertion_point(field_set:gate.GateSSMsg.uid)
}

// required uint32 area = 4;
inline bool GateSSMsg::has_area() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void GateSSMsg::set_has_area() {
  _has_bits_[0] |= 0x00000008u;
}
inline void GateSSMsg::clear_has_area() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void GateSSMsg::clear_area() {
  area_ = 0u;
  clear_has_area();
}
inline ::google::protobuf::uint32 GateSSMsg::area() const {
  // @@protoc_insertion_point(field_get:gate.GateSSMsg.area)
  return area_;
}
inline void GateSSMsg::set_area(::google::protobuf::uint32 value) {
  set_has_area();
  area_ = value;
  // @@protoc_insertion_point(field_set:gate.GateSSMsg.area)
}

// optional uint32 ip = 20;
inline bool GateSSMsg::has_ip() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void GateSSMsg::set_has_ip() {
  _has_bits_[0] |= 0x00000010u;
}
inline void GateSSMsg::clear_has_ip() {
  _has_bits_[0] &= ~0x00000010u;
}
inline void GateSSMsg::clear_ip() {
  ip_ = 0u;
  clear_has_ip();
}
inline ::google::protobuf::uint32 GateSSMsg::ip() const {
  // @@protoc_insertion_point(field_get:gate.GateSSMsg.ip)
  return ip_;
}
inline void GateSSMsg::set_ip(::google::protobuf::uint32 value) {
  set_has_ip();
  ip_ = value;
  // @@protoc_insertion_point(field_set:gate.GateSSMsg.ip)
}

// optional uint32 port = 21;
inline bool GateSSMsg::has_port() const {
  return (_has_bits_[0] & 0x00000020u) != 0;
}
inline void GateSSMsg::set_has_port() {
  _has_bits_[0] |= 0x00000020u;
}
inline void GateSSMsg::clear_has_port() {
  _has_bits_[0] &= ~0x00000020u;
}
inline void GateSSMsg::clear_port() {
  port_ = 0u;
  clear_has_port();
}
inline ::google::protobuf::uint32 GateSSMsg::port() const {
  // @@protoc_insertion_point(field_get:gate.GateSSMsg.port)
  return port_;
}
inline void GateSSMsg::set_port(::google::protobuf::uint32 value) {
  set_has_port();
  port_ = value;
  // @@protoc_insertion_point(field_set:gate.GateSSMsg.port)
}

// optional uint32 reason = 22;
inline bool GateSSMsg::has_reason() const {
  return (_has_bits_[0] & 0x00000040u) != 0;
}
inline void GateSSMsg::set_has_reason() {
  _has_bits_[0] |= 0x00000040u;
}
inline void GateSSMsg::clear_has_reason() {
  _has_bits_[0] &= ~0x00000040u;
}
inline void GateSSMsg::clear_reason() {
  reason_ = 0u;
  clear_has_reason();
}
inline ::google::protobuf::uint32 GateSSMsg::reason() const {
  // @@protoc_insertion_point(field_get:gate.GateSSMsg.reason)
  return reason_;
}
inline void GateSSMsg::set_reason(::google::protobuf::uint32 value) {
  set_has_reason();
  reason_ = value;
  // @@protoc_insertion_point(field_set:gate.GateSSMsg.reason)
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace gate

#ifndef SWIG
namespace google {
namespace protobuf {

template <> struct is_proto_enum< ::gate::GateSSMsg_EventType> : ::google::protobuf::internal::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::gate::GateSSMsg_EventType>() {
  return ::gate::GateSSMsg_EventType_descriptor();
}
template <> struct is_proto_enum< ::gate::GateSSMsg_ConnectionCloseReason> : ::google::protobuf::internal::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::gate::GateSSMsg_ConnectionCloseReason>() {
  return ::gate::GateSSMsg_ConnectionCloseReason_descriptor();
}

}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_gate_2fGateSSMsg_2eproto__INCLUDED
