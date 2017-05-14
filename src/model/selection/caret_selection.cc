#include "caret_selection.h"

#include "model/character/enter_character.h"
#include "model/character/character.h"
#include "controller/caret_controller.h"
#include <memory>

namespace le {


CaretSelection::CaretSelection(Character& character)
    : character_(character),
      view_(*this)
{
  
}

CaretSelection::~CaretSelection()
{}

std::unique_ptr<Controller> CaretSelection::GetController(RenderText* rendertext) const
{
  return std::unique_ptr<Controller>(new CaretController(character_, rendertext));
}

View& CaretSelection::GetView() 
{
  return view_;
}


}  // le
