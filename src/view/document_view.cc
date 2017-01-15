#include "document_view.h"

#include "view/page/page_view.h"
#include "model/document.h"

namespace le {

DocumentView::DocumentView(Document& document)
    : document_(document)
{}

DocumentView::~DocumentView()
{}

void DocumentView::createPageViews()
{
  auto page = new PageView(GetDocument().GetSectionProp());
  AddChildAt(0, page);

  for( auto paragraph = GetDocument().GetLastChild() ; paragraph ;
       paragraph = static_cast<Paragraph*>(paragraph->GetPrevSibling()) )
  {
    page->AddChildAt(0, &paragraph->GetView());
  }
  
}


}  // le
