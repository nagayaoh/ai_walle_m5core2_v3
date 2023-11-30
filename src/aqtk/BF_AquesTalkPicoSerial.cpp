// Copyright 2021 BotanicFields, Inc.
// BF-034 AquesTalk pico LSI Module for M5Stack

#include <Arduino.h>
#include "BF_AquesTalkPicoSerial.h"

AquesTalkPicoSerial::AquesTalkPicoSerial()
{
  m_recv = new char[m_recv_size];
  for (int i = 0; i < m_recv_size; ++i)
    m_recv[i] = 0;
}

AquesTalkPicoSerial::~AquesTalkPicoSerial()
{
  delete[] m_recv;
}

int AquesTalkPicoSerial::Begin(Stream &stream)
{
  delay(80);  // 80ms: reset process of AquesTalk-Pico
  m_stream = &stream;
  m_recv_count = 0;
  return 0;
}

int AquesTalkPicoSerial::Send(const char* msg)
{
  Serial.printf("AquesTalk-Pico Serial > Send: %s\n", msg);
  m_stream->write(msg);
  return 0;
}

size_t AquesTalkPicoSerial::Recv(char* res, size_t res_size)
{
  while (m_recv_count <  m_recv_size - 1) {
    if (m_stream->available()) {
      char recv_data = m_stream->read();
      if (recv_data >= 0 ){
        m_recv[m_recv_count++] = recv_data;
        if (recv_data == '>' || recv_data == '*')
          break;
      }else{
        Serial.println("AquesTalk-Pico Serial > Recv: Error");
        return 0;
      }
    }
    else
      return 0;
  }
  if (m_recv_count > res_size - 1)
    m_recv_count = res_size - 1;

  for (int i = 0; i < m_recv_count; ++i)
    res[i] = m_recv[i];
  res[m_recv_count] = '\0';
  m_recv_count = 0;

  Serial.printf("AquesTalk-Pico Serial > Recv: %s\n", res);

  return strlen(res);
}

bool AquesTalkPicoSerial::Busy()
{
//   Send("\r");
//   char res[10];
//   int res_length(0);
//   do {
//     res_length = Recv(res, sizeof(res));
//     Serial.printf("[AquesTalk Serial] Receive res_length:%d\n", res_length);
//   } while (res_length == 0);
//   if (res_length >= 2) {
//     Serial.printf("[AquesTalk Serial] Receive:%s\n", res);
//     return true;
//   }
//   if (res[0] != '>')
//     return true;
//   return false;

  // Send("\r");
  char res[10];
  int res_length(0);

  if (m_stream->available()) {
    int intChar = m_stream->peek();
    Serial.printf("[AquesTalk Serial] Receive:%c\n", intChar);
    if (intChar == '*'){
      m_stream->read();
      return true;
    }
  }

  Serial.printf("[AquesTalk Serial] Recv is Vacant\n");
  return false;
}