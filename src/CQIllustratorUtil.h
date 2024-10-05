#ifndef CQIllustratorUtil_H
#define CQIllustratorUtil_H

#include <CQIllustratorSelection.h>
#include <CQIllustrator.h>

namespace CQIllustratorUtil {
  template<typename T>
  T *getCurrentShape(CQIllustrator *illustrator) {
    T *shape = nullptr;

    CQIllustratorSelectedShapes *selection = illustrator->getSelection();

    CQIllustratorSelectedShapes::iterator ps1, ps2;

    for (ps1 = selection->begin(), ps2 = selection->end(); ps1 != ps2; ++ps1) {
      CQIllustratorShape *shape1 = (*ps1).getShape();

      T *tshape = dynamic_cast<T *>(shape1);

      if (tshape == nullptr) continue;

      if (shape == nullptr)
        shape = tshape;
      else
        break;
    }

    return shape;
  }
}

#endif
