#include "mednafen/mednafen-types.h"
#include "mednafen/mednafen.h"
#include "mednafen/md5.h"
#include "mednafen/git.h"
#include "mednafen/general.h"
#include "mednafen/mednafen-driver.h"

#include <iostream>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

// Stubs

void MDFND_Sleep(unsigned int time)
{
#ifdef _WIN32
   Sleep(time);
#else
   usleep(time * 1000);
#endif
}

extern std::string retro_base_directory;
extern std::string retro_base_name;

// Use a simpler approach to make sure that things go right for libretro.
std::string MDFN_MakeFName(MakeFName_Type type, int id1, const char *cd1)
{
   std::string ret;
   switch (type)
   {
      case MDFNMKF_SAV:
         ret = retro_base_directory +
            std::string(PSS) +
            retro_base_name +
            std::string(".") +
            md5_context::asciistr(MDFNGameInfo->MD5, 0) +
            std::string(".") +
            std::string(cd1);
         break;
      case MDFNMKF_FIRMWARE:
         ret = std::string(cd1);
         break;
      default:
         break;
   }

   fprintf(stderr, "[Mednafen]: Path request: %s\n", ret.c_str());
   return ret;
}

void MDFND_DispMessage(unsigned char *str)
{
   std::cerr << str;
}

void MDFND_Message(const char *str)
{
   std::cerr << str;
}

void MDFND_MidSync(const EmulateSpecStruct *)
{}

void MDFND_PrintError(const char* err)
{
   std::cerr << err;
}

uint32 MDFND_GetTime()
{
   static bool first = true;
   static uint32_t start_ms;

#ifdef _WIN32
   DWORD ms = timeGetTime();
   if (first)
   {
      start_ms = ms;
      first = false;
   }
#else
   struct timeval val;
   gettimeofday(&val, NULL);
   uint32_t ms = val.tv_sec * 1000 + val.tv_usec / 1000;

   if (first)
   {
      start_ms = ms;
      first = false;
   }
#endif

   return ms - start_ms;
}

