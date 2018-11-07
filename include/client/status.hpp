
/*
 * These are generic utility classes and functions within the Platform Layer.
 */
#ifndef GOOGLEAPI_CLIENT_STATUS_H_
#define GOOGLEAPI_CLIENT_STATUS_H_

#include <schedule/api/status.hpp>
#include <string>

using std::string;

namespace googleapi {
namespace client {

/*
 * Shorthand notation for creating a status from a standard util::error enum
 * The symbol parameter is the symbolic enum name with the util::error
 * namespace stripped from it.
 */
#define STATUS_FROM_ENUM(symbol, msg) \
  googleapi::util::Status(util::error::symbol, msg)

/*
 * Creates a standard OK status.
 */
inline googleapi::util::Status  StatusOk() { return googleapi::util::Status(); }

/*
 * Creates a standard ABORTED status.
 */
inline googleapi::util::Status StatusAborted(const string& msg) {
  return STATUS_FROM_ENUM(ABORTED, msg);
}

/*
 * Creates a standard CANCELLED status.
 */
inline googleapi::util::Status StatusCanceled(const string& msg) {
  return STATUS_FROM_ENUM(CANCELLED, msg);
}

/*
 * Creates a standard DATA_LOSS status.
 */
inline googleapi::util::Status StatusDataLoss(const string& msg) {
  return STATUS_FROM_ENUM(DATA_LOSS, msg);
}

/*
 * Creates a standard DEADLINE_EXCEEDED status.
 */
inline googleapi::util::Status StatusDeadlineExceeded(const string& msg) {
  return STATUS_FROM_ENUM(DEADLINE_EXCEEDED, msg);
}

/*
 * Creates a standard INTERNAL status.
 */
inline googleapi::util::Status StatusInternalError(const string& msg) {
  return STATUS_FROM_ENUM(INTERNAL, msg);
}

/*
 * Creates a standard INVALID_ARGUMENT status.
 */
inline googleapi::util::Status StatusInvalidArgument(const string& msg) {
  return STATUS_FROM_ENUM(INVALID_ARGUMENT, msg);
}

/*
 * Creates a standard OUT_OF_RANGE status.
 */
inline googleapi::util::Status StatusOutOfRange(const string& msg) {
  return STATUS_FROM_ENUM(OUT_OF_RANGE, msg);
}

/*
 * Creates a standard PERMISSION_DENIED status.
 */
inline googleapi::util::Status StatusPermissionDenied(const string& msg) {
  return STATUS_FROM_ENUM(PERMISSION_DENIED, msg);
}

/*
 * Creates a standard UNIMPLEMENTED status.
 */
inline googleapi::util::Status StatusUnimplemented(const string& msg) {
  return STATUS_FROM_ENUM(UNIMPLEMENTED, msg);
}

/*
 * Creates a standard UNKNOWN status.
 */
inline googleapi::util::Status StatusUnknown(const string& msg) {
  return STATUS_FROM_ENUM(UNKNOWN, msg);
}

/*
 * Creates a standard RESOURCE_EXHAUSTED status.
 */
inline googleapi::util::Status StatusResourceExhausted(const string& msg) {
  return STATUS_FROM_ENUM(RESOURCE_EXHAUSTED, msg);
}

/*
 * Creates a standard FAILED_PRECONDITION status.
 */
inline googleapi::util::Status StatusFailedPrecondition(const string& msg) {
  return STATUS_FROM_ENUM(FAILED_PRECONDITION, msg);
}

}  // namespace client

}  // namespace googleapi

#endif  // GOOGLEAPI_CLIENT_STATUS_H_
