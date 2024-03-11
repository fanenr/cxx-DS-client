#ifndef UTIL_H
#define UTIL_H

#define URL_BASE "http://127.0.0.1:8000/api"
#define URL_BASE_STUDENT URL_BASE "/student"
#define URL_BASE_MERCHANT URL_BASE "/merchant"
#define URL_BASE_MENU URL_BASE "/menu"

#define URL_STUDENT_NEW URL_BASE_STUDENT "/new"
#define URL_STUDENT_LOG URL_BASE_STUDENT "/log"
#define URL_STUDENT_MOD URL_BASE_STUDENT "/mod"
#define URL_STUDENT_DEL URL_BASE_STUDENT "/del"

#define URL_MERCHANT_NEW URL_BASE_MERCHANT "/new"
#define URL_MERCHANT_LOG URL_BASE_MERCHANT "/log"
#define URL_MERCHANT_MOD URL_BASE_MERCHANT "/mod"
#define URL_MERCHANT_DEL URL_BASE_MERCHANT "/del"

#define URL_MENU_LIST URL_BASE_MENU "/list"
#define URL_MENU_NEW URL_BASE_MENU "/new"
#define URL_MENU_MOD URL_BASE_MENU "/mod"
#define URL_MENU_DEL URL_BASE_MENU "/del"

enum class type
{
  STUDENT,
  MERCHANT,
};

#endif