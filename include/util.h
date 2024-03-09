#ifndef UTIL_H
#define UTIL_H

#define URL_BASE "http://127.0.0.1:8000/api"

#define URL_STUDENT_NEW URL_BASE "/student/new"
#define URL_MERCHANT_NEW URL_BASE "/merchant/new"

#define URL_STUDENT_LOG URL_BASE "/student/log"
#define URL_MERCHANT_LOG URL_BASE "/merchant/log"

enum class type
{
  NONE,
  STUDENT,
  MERCHANT,
};

#endif