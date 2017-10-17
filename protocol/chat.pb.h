// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: chat.proto

#ifndef PROTOBUF_chat_2eproto__INCLUDED
#define PROTOBUF_chat_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3001000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3001000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace chat {

// Internal implementation detail -- do not call these.
void protobuf_AddDesc_chat_2eproto();
void protobuf_InitDefaults_chat_2eproto();
void protobuf_AssignDesc_chat_2eproto();
void protobuf_ShutdownFile_chat_2eproto();

class msg_chat_reply;
class msg_chat_request;

// ===================================================================

class msg_chat_request : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:chat.msg_chat_request) */ {
 public:
  msg_chat_request();
  virtual ~msg_chat_request();

  msg_chat_request(const msg_chat_request& from);

  inline msg_chat_request& operator=(const msg_chat_request& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const msg_chat_request& default_instance();

  static const msg_chat_request* internal_default_instance();

  void Swap(msg_chat_request* other);

  // implements Message ----------------------------------------------

  inline msg_chat_request* New() const { return New(NULL); }

  msg_chat_request* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const msg_chat_request& from);
  void MergeFrom(const msg_chat_request& from);
  void Clear();
  bool IsInitialized() const;

  size_t ByteSizeLong() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const {
    return InternalSerializeWithCachedSizesToArray(false, output);
  }
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(msg_chat_request* other);
  void UnsafeMergeFrom(const msg_chat_request& from);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional int32 device = 1;
  void clear_device();
  static const int kDeviceFieldNumber = 1;
  ::google::protobuf::int32 device() const;
  void set_device(::google::protobuf::int32 value);

  // optional int32 from_uid = 2;
  void clear_from_uid();
  static const int kFromUidFieldNumber = 2;
  ::google::protobuf::int32 from_uid() const;
  void set_from_uid(::google::protobuf::int32 value);

  // optional int32 to_uid = 3;
  void clear_to_uid();
  static const int kToUidFieldNumber = 3;
  ::google::protobuf::int32 to_uid() const;
  void set_to_uid(::google::protobuf::int32 value);

  // optional string msg = 4;
  void clear_msg();
  static const int kMsgFieldNumber = 4;
  const ::std::string& msg() const;
  void set_msg(const ::std::string& value);
  void set_msg(const char* value);
  void set_msg(const char* value, size_t size);
  ::std::string* mutable_msg();
  ::std::string* release_msg();
  void set_allocated_msg(::std::string* msg);

  // @@protoc_insertion_point(class_scope:chat.msg_chat_request)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::ArenaStringPtr msg_;
  ::google::protobuf::int32 device_;
  ::google::protobuf::int32 from_uid_;
  ::google::protobuf::int32 to_uid_;
  mutable int _cached_size_;
  friend void  protobuf_InitDefaults_chat_2eproto_impl();
  friend void  protobuf_AddDesc_chat_2eproto_impl();
  friend void protobuf_AssignDesc_chat_2eproto();
  friend void protobuf_ShutdownFile_chat_2eproto();

  void InitAsDefaultInstance();
};
extern ::google::protobuf::internal::ExplicitlyConstructed<msg_chat_request> msg_chat_request_default_instance_;

// -------------------------------------------------------------------

class msg_chat_reply : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:chat.msg_chat_reply) */ {
 public:
  msg_chat_reply();
  virtual ~msg_chat_reply();

  msg_chat_reply(const msg_chat_reply& from);

  inline msg_chat_reply& operator=(const msg_chat_reply& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const msg_chat_reply& default_instance();

  static const msg_chat_reply* internal_default_instance();

  void Swap(msg_chat_reply* other);

  // implements Message ----------------------------------------------

  inline msg_chat_reply* New() const { return New(NULL); }

  msg_chat_reply* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const msg_chat_reply& from);
  void MergeFrom(const msg_chat_reply& from);
  void Clear();
  bool IsInitialized() const;

  size_t ByteSizeLong() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const {
    return InternalSerializeWithCachedSizesToArray(false, output);
  }
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(msg_chat_reply* other);
  void UnsafeMergeFrom(const msg_chat_reply& from);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional int32 from_uid = 1;
  void clear_from_uid();
  static const int kFromUidFieldNumber = 1;
  ::google::protobuf::int32 from_uid() const;
  void set_from_uid(::google::protobuf::int32 value);

  // optional int32 to_uid = 2;
  void clear_to_uid();
  static const int kToUidFieldNumber = 2;
  ::google::protobuf::int32 to_uid() const;
  void set_to_uid(::google::protobuf::int32 value);

  // optional int32 result = 3;
  void clear_result();
  static const int kResultFieldNumber = 3;
  ::google::protobuf::int32 result() const;
  void set_result(::google::protobuf::int32 value);

  // optional string msg = 4;
  void clear_msg();
  static const int kMsgFieldNumber = 4;
  const ::std::string& msg() const;
  void set_msg(const ::std::string& value);
  void set_msg(const char* value);
  void set_msg(const char* value, size_t size);
  ::std::string* mutable_msg();
  ::std::string* release_msg();
  void set_allocated_msg(::std::string* msg);

  // @@protoc_insertion_point(class_scope:chat.msg_chat_reply)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::ArenaStringPtr msg_;
  ::google::protobuf::int32 from_uid_;
  ::google::protobuf::int32 to_uid_;
  ::google::protobuf::int32 result_;
  mutable int _cached_size_;
  friend void  protobuf_InitDefaults_chat_2eproto_impl();
  friend void  protobuf_AddDesc_chat_2eproto_impl();
  friend void protobuf_AssignDesc_chat_2eproto();
  friend void protobuf_ShutdownFile_chat_2eproto();

  void InitAsDefaultInstance();
};
extern ::google::protobuf::internal::ExplicitlyConstructed<msg_chat_reply> msg_chat_reply_default_instance_;

// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// msg_chat_request

// optional int32 device = 1;
inline void msg_chat_request::clear_device() {
  device_ = 0;
}
inline ::google::protobuf::int32 msg_chat_request::device() const {
  // @@protoc_insertion_point(field_get:chat.msg_chat_request.device)
  return device_;
}
inline void msg_chat_request::set_device(::google::protobuf::int32 value) {
  
  device_ = value;
  // @@protoc_insertion_point(field_set:chat.msg_chat_request.device)
}

// optional int32 from_uid = 2;
inline void msg_chat_request::clear_from_uid() {
  from_uid_ = 0;
}
inline ::google::protobuf::int32 msg_chat_request::from_uid() const {
  // @@protoc_insertion_point(field_get:chat.msg_chat_request.from_uid)
  return from_uid_;
}
inline void msg_chat_request::set_from_uid(::google::protobuf::int32 value) {
  
  from_uid_ = value;
  // @@protoc_insertion_point(field_set:chat.msg_chat_request.from_uid)
}

// optional int32 to_uid = 3;
inline void msg_chat_request::clear_to_uid() {
  to_uid_ = 0;
}
inline ::google::protobuf::int32 msg_chat_request::to_uid() const {
  // @@protoc_insertion_point(field_get:chat.msg_chat_request.to_uid)
  return to_uid_;
}
inline void msg_chat_request::set_to_uid(::google::protobuf::int32 value) {
  
  to_uid_ = value;
  // @@protoc_insertion_point(field_set:chat.msg_chat_request.to_uid)
}

// optional string msg = 4;
inline void msg_chat_request::clear_msg() {
  msg_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& msg_chat_request::msg() const {
  // @@protoc_insertion_point(field_get:chat.msg_chat_request.msg)
  return msg_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void msg_chat_request::set_msg(const ::std::string& value) {
  
  msg_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:chat.msg_chat_request.msg)
}
inline void msg_chat_request::set_msg(const char* value) {
  
  msg_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:chat.msg_chat_request.msg)
}
inline void msg_chat_request::set_msg(const char* value, size_t size) {
  
  msg_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:chat.msg_chat_request.msg)
}
inline ::std::string* msg_chat_request::mutable_msg() {
  
  // @@protoc_insertion_point(field_mutable:chat.msg_chat_request.msg)
  return msg_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* msg_chat_request::release_msg() {
  // @@protoc_insertion_point(field_release:chat.msg_chat_request.msg)
  
  return msg_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void msg_chat_request::set_allocated_msg(::std::string* msg) {
  if (msg != NULL) {
    
  } else {
    
  }
  msg_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), msg);
  // @@protoc_insertion_point(field_set_allocated:chat.msg_chat_request.msg)
}

inline const msg_chat_request* msg_chat_request::internal_default_instance() {
  return &msg_chat_request_default_instance_.get();
}
// -------------------------------------------------------------------

// msg_chat_reply

// optional int32 from_uid = 1;
inline void msg_chat_reply::clear_from_uid() {
  from_uid_ = 0;
}
inline ::google::protobuf::int32 msg_chat_reply::from_uid() const {
  // @@protoc_insertion_point(field_get:chat.msg_chat_reply.from_uid)
  return from_uid_;
}
inline void msg_chat_reply::set_from_uid(::google::protobuf::int32 value) {
  
  from_uid_ = value;
  // @@protoc_insertion_point(field_set:chat.msg_chat_reply.from_uid)
}

// optional int32 to_uid = 2;
inline void msg_chat_reply::clear_to_uid() {
  to_uid_ = 0;
}
inline ::google::protobuf::int32 msg_chat_reply::to_uid() const {
  // @@protoc_insertion_point(field_get:chat.msg_chat_reply.to_uid)
  return to_uid_;
}
inline void msg_chat_reply::set_to_uid(::google::protobuf::int32 value) {
  
  to_uid_ = value;
  // @@protoc_insertion_point(field_set:chat.msg_chat_reply.to_uid)
}

// optional int32 result = 3;
inline void msg_chat_reply::clear_result() {
  result_ = 0;
}
inline ::google::protobuf::int32 msg_chat_reply::result() const {
  // @@protoc_insertion_point(field_get:chat.msg_chat_reply.result)
  return result_;
}
inline void msg_chat_reply::set_result(::google::protobuf::int32 value) {
  
  result_ = value;
  // @@protoc_insertion_point(field_set:chat.msg_chat_reply.result)
}

// optional string msg = 4;
inline void msg_chat_reply::clear_msg() {
  msg_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& msg_chat_reply::msg() const {
  // @@protoc_insertion_point(field_get:chat.msg_chat_reply.msg)
  return msg_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void msg_chat_reply::set_msg(const ::std::string& value) {
  
  msg_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:chat.msg_chat_reply.msg)
}
inline void msg_chat_reply::set_msg(const char* value) {
  
  msg_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:chat.msg_chat_reply.msg)
}
inline void msg_chat_reply::set_msg(const char* value, size_t size) {
  
  msg_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:chat.msg_chat_reply.msg)
}
inline ::std::string* msg_chat_reply::mutable_msg() {
  
  // @@protoc_insertion_point(field_mutable:chat.msg_chat_reply.msg)
  return msg_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* msg_chat_reply::release_msg() {
  // @@protoc_insertion_point(field_release:chat.msg_chat_reply.msg)
  
  return msg_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void msg_chat_reply::set_allocated_msg(::std::string* msg) {
  if (msg != NULL) {
    
  } else {
    
  }
  msg_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), msg);
  // @@protoc_insertion_point(field_set_allocated:chat.msg_chat_reply.msg)
}

inline const msg_chat_reply* msg_chat_reply::internal_default_instance() {
  return &msg_chat_reply_default_instance_.get();
}
#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace chat

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_chat_2eproto__INCLUDED
