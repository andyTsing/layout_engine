#include "server.h"

#include <stdio.h>
#include <stdlib.h>
#include <cstring>

#include <iostream>
#include <string>
#include <string.h>
#include <algorithm>
#include <thread>
#include <memory>
#include <jsoncpp/json/json.h>

#include "render_text.h"
#include "graphic/bitmap.h"
#include "graphic/canvas.h"
#include "controller/event/mouse_event.h"
#include "controller/event/key_event.h"

namespace le {
namespace web {

namespace {

void PngWriteCallback(png_structp png_ptr, png_bytep data, png_size_t length)
{
  std::vector<char> *p = (std::vector<char>*)png_get_io_ptr(png_ptr);
  p->insert(p->end(), data, data + length);
}

std::vector<char> Buffer2Png(int width, int height, char* buffer)
{
  std::vector<char> out;
  
  png_structp p = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  png_infop info_ptr = png_create_info_struct(p);
  setjmp(png_jmpbuf(p));
  png_set_IHDR(p, info_ptr, width, height, 8,
               PNG_COLOR_TYPE_RGB,
               PNG_INTERLACE_NONE,
               PNG_COMPRESSION_TYPE_DEFAULT,
               PNG_FILTER_TYPE_DEFAULT);


  std::vector<char*> rows(height);
  for (size_t y = 0; y < height; ++y)
    rows[y] = buffer + y * width * 3;

  //png_set_compression_level(p, 1);
  png_set_rows(p, info_ptr, (unsigned char**)&rows[0]);
  png_set_write_fn(p, &out, PngWriteCallback, NULL);

  png_write_png(p, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

  return out;
}


bool ProcessEvent(const std::string& json, le::RenderText& rendertext)
{

  Json::Value root;
  Json::Reader reader;

  if( !reader.parse(json, root) )
    return false;

  auto event_type = root.get("type", "" ).asString();
  if( event_type == "keypress" ) {
    // auto key = root.get("key", "" ).asString();
    // std::cout << "received key : " <<key<< std::endl;
    // rendertext.InsertText(key);
    // rendertext.Layout();
    // return true;
    
  } else if( event_type == "keydown" ) {
    auto code = root.get("keyCode", "" ).asInt();
    auto ctrl = root.get("ctrlKey", "false" ).asBool();
    auto alt = root.get("altKey", "false" ).asBool();
    auto shift = root.get("shiftKey", "false" ).asBool();

    std::cout << "keydown : " << code << std::endl;
    
    le::KeyEvent keyevent;
    auto str = root.get("key", "" ).asString();
    if( str.length() == 1 )
      keyevent.SetChar(str[0]);
    
    keyevent.SetCode(static_cast<le::KeyboardCode>(code));
    keyevent.SetCtrlDown(ctrl);
    keyevent.SetAltDown(alt);
    keyevent.SetShiftDown(shift);

    std::cout << "before  : " << code << std::endl;
    
    rendertext.OnKeyDown(keyevent);

    std::cout << "layout  : " << code << std::endl;
    rendertext.Layout();
    return true;
    
  } else if ( event_type == "mousedown" ) {
    
    auto x = root.get("x", "0" ).asInt();
    auto y = root.get("y", "0" ).asInt();

    le::MouseEvent event;
    event.SetX(x);
    event.SetY(y);

    rendertext.OnMousePressed(event);
    return false;
  }
  
  
}


int callback_http(lws* wsi,
                  lws_callback_reasons reason,
                  void *user,
                  void *in, size_t len)
{
  switch (reason) {
    case LWS_CALLBACK_CLIENT_WRITEABLE:
      printf("connection client established\n");
      break;
    case LWS_CALLBACK_HTTP: {
      char *requested_uri = (char *) in;
      printf("requested URI: %s\n", requested_uri);

      lws_serve_http_file(wsi, "index.html", "text/html", nullptr, 0);
      break;
    }

    case LWS_CALLBACK_ESTABLISHED: {
      auto rendertext_map = static_cast<std::unordered_map<lws*, RenderText*>*>(lws_get_protocol(wsi)->user);
      printf("connection established\n");
      auto rendertext = new RenderText();

      std::cout << "user : " << user << std::endl;

      rendertext_map->emplace(wsi, rendertext);
      break;
    }
    case LWS_CALLBACK_CLOSED: {
      auto rendertext_map = static_cast<std::unordered_map<lws*, RenderText*>*>(lws_get_protocol(wsi)->user);
      printf("connection closed\n");
      rendertext_map->erase(wsi);
      break;
    }
    case LWS_CALLBACK_RECEIVE: {
      auto rendertext_map = static_cast<std::unordered_map<lws*, RenderText*>*>(lws_get_protocol(wsi)->user);
      std::string str = reinterpret_cast<char*>(in);
      std::cout << str << std::endl;
      auto it = rendertext_map->find(wsi);
      if( it != rendertext_map->end() ) {
        auto& rendertext = *it->second;
        ProcessEvent(str, rendertext);
        auto& document_view = rendertext.GetDocument().GetView();
        auto bitmap = new le::Bitmap(document_view.GetWidth(), document_view.GetHeight(), 3);

        std::cout << "painting...";
        le::Canvas canvas;
        canvas.SetBitmap(bitmap);
        rendertext.Paint(canvas);
        std::cout << "complete!" <<std::endl;

        auto data = bitmap->GetData();
        auto size = bitmap->GetWidth() * bitmap->GetHeight() * bitmap->GetDepth();

        // bitmap->WriteBitmapFile("output.bmp");

        auto bitmap_width = bitmap->GetWidth();
        auto bitmap_height = bitmap->GetHeight();
        auto bitmap_depth = bitmap->GetDepth();

        std::cout << "compress...";
        auto out = Buffer2Png(bitmap_width, bitmap_height, (char*)data);
        std::cout << "bufferlen:" <<out.size() << "complete!" <<std::endl;

        std::unique_ptr<unsigned char[]> send_data(new unsigned char[LWS_PRE + out.size()]);
        //unsigned char* send_data = new unsigned char[LWS_PRE + out.size()];
        std::memcpy(&send_data.get()[LWS_PRE], out.data(), out.size());
        

        lws_write(wsi, (unsigned char*)&send_data.get()[LWS_PRE], out.size(), LWS_WRITE_BINARY);

        //delete[] send_data;
        
      }

      
      break;
    }
    case LWS_CALLBACK_GET_THREAD_ID: {
      std::cout << "thread!!" << std::endl;
      std::cout<< "pid : " << getpid() << std::endl;
      return getpid();
    }
    case LWS_CALLBACK_CHANGE_MODE_POLL_FD:
      std::cout << "change mode pool\n" << std::endl;
      break;
    case LWS_CALLBACK_LOCK_POLL:
      printf("lock poll %d\n", reason);
      break;
    case LWS_CALLBACK_UNLOCK_POLL:
      printf("unlock poll %d\n", reason);
      break;
    case LWS_CALLBACK_ADD_POLL_FD:
      printf("add pool fd %d\n", reason);
      break;
    case LWS_CALLBACK_DEL_POLL_FD:
      printf("delete pool fd %d\n", reason);
      break;
      
    default:
      printf("unhandled callback %d\n", reason);
      break;
  }

  return 0;
}

lws_protocols protocols[] = {
  /* first protocol must always be HTTP handler */
  {
    "le_web_protocol",
    callback_http,
    0,
    0,
  },
  {
    NULL, NULL, 0   /* End of list */
  }
};


}

Server::Server()
{

}

Server::~Server()
{

}

void Server::Start()
{
  int port = 8080;
  const char *interface = NULL;
  lws_context *context;
  int opts = 0;

  lws_protocols protocol;
  memset(&protocol, 0, sizeof(protocol));
  protocol.name = "le_web_protocol";
  protocol.callback = callback_http;
  protocol.user = &rendertext_map_;

  lws_context_creation_info info;
  memset(&info, 0, sizeof(info));
  
  info.port = port;
  info.iface = interface;
  info.protocols = &protocol;
  info.extensions = nullptr;
  //if (!use_ssl) {
  info.ssl_cert_filepath = NULL;
  info.ssl_private_key_filepath = NULL;
  //} else {
  //  info.ssl_cert_filepath = LOCAL_RESOURCE_PATH"/libwebsockets-test-server.pem";
  //  info.ssl_private_key_filepath = LOCAL_RESOURCE_PATH"/libwebsockets-test-server.key.pem";
  //}
  info.gid = -1;
  info.uid = -1;
  info.options = opts;

  context = lws_create_context(&info);

  le::RenderText rendertext;
  

  if (context == NULL) {
    fprintf(stderr, "libwebsocket init failed\n");
    return;
  }

  printf("starting server...\n");

  // infinite loop, to end this server send SIGTERM. (CTRL+C)
  while (1) {
    auto n = lws_service(context, 50);
  }

  lws_context_destroy(context);
}


}  // web
}  // le