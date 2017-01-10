#include "line_view.h"

#include "view/word_view.h"
#include "view/run/run_view.h"
#include "graphic/canvas.h"
#include "view/character/enter_character_view.h"
#include "view/character/character_view.h"
#include "model/word.h"
#include "model/character/character.h"

namespace le {

LineView::LineView()
{}

LineView::~LineView()
{}

void LineView::Paint(Canvas& canvas)
{
  canvas.Save();
  canvas.GetMatrix().Translate(GetX(), GetY());
  
  for( auto index = 0; index < GetChildCount() ; ++index )
    GetChildAt(index)->Paint(canvas);

  Point p1(0, GetBaseline());
  Point p2(GetWidth(), GetBaseline());
  canvas.DrawLine(p1, p2);

  canvas.Restore();
}

void LineView::Layout()
{
  SetX(0);
  SetWidth(GetParent()->GetWidth());

  auto line_index = GetIndex();
  if( line_index == 0 ) {
    SetY(0);
  }
  else {
    auto prev = GetParent()->GetChildAt(line_index - 1);
    SetY(prev->GetY() + prev->GetHeight());
  }

  for( auto index = 0; index < GetChildCount() ; ++index )
    GetChildAt(index)->Layout();
  
  auto maxheight = 0;
  auto baseline = 0;

  for( auto index = 0; index < GetChildCount() ; ++index ) {
    if( typeid(*GetChildAt(index)) == typeid(WordView) ) {
      auto child = static_cast<WordView*>(GetChildAt(index));
      maxheight = std::max( maxheight, child->GetHeight() );
      baseline = std::max(baseline, child->GetWord().GetWordAscender());
      
    } else if ( typeid(*GetChildAt(index)) == typeid(EnterCharacterView) ) {
      auto enter_char_view = static_cast<EnterCharacterView*>(GetChildAt(index));
      auto& character = enter_char_view->GetCharacter();
      auto enter_run = character.GetRun();
    } else if ( typeid(*GetChildAt(index)) == typeid(RunView) ) {
      auto runview = static_cast<RunView*>(GetChildAt(index));
      
    }
  }

  SetHeight(maxheight);
  SetBaseline(baseline);
}

View* LineView::GetNextParent() const
{
  auto nextline = GetNextSibling();
  if( !nextline ) {
    nextline = new LineView();
    GetParent()->AddChildAt(GetParent()->GetChildCount(), nextline);
  }

  return nextline;
}

int LineView::GetAvailableWidth() const
{
  auto parent = GetParent();
  if( parent ) {
    return parent->GetAvailableWidth();
  } else {
    return GetWidth();
  }
}
    

}  // le
