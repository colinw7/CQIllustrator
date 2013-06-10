#ifndef CEVENT_H
#define CEVENT_H

#include <sys/types.h>
#include <iostream>
#include <string>
#include <CIPoint2D.h>
#include <CKeyType.h>

enum CButtonAction {
  CBUTTON_ACTION_NONE    = 0,
  CBUTTON_ACTION_PRESS   = 1,
  CBUTTON_ACTION_MOTION  = 2,
  CBUTTON_ACTION_RELEASE = 3
};

enum CEventModifier {
  CMODIFIER_NONE    = 0,
  CMODIFIER_SHIFT   = (1<<0),
  CMODIFIER_CONTROL = (1<<1),
  CMODIFIER_ALT     = (1<<2),
  CMODIFIER_META    = (1<<3),
  CMODIFIER_KEYPAD  = (1<<4)
};

enum CMouseButton {
  CBUTTON_NONE   = 0,
  CBUTTON_LEFT   = 1,
  CBUTTON_MIDDLE = 2,
  CBUTTON_RIGHT  = 3
};

class CEvent {
 public:
  static std::string keyTypeName(CKeyType type);
  static char        keyTypeChar(CKeyType type);
  static CKeyType    charKeyType(char c);
  static bool        keyTypeIsAlpha(CKeyType type);
  static bool        keyTypeIsDigit(CKeyType type);

  CEvent() { }

  virtual ~CEvent() { }
};

class CMouseEvent : public CEvent {
 public:
  CMouseEvent() :
   CEvent(), position_(0,0), button_(CBUTTON_NONE), count_(0), modifier_(CMODIFIER_NONE) {
  }

  CMouseEvent(const CIPoint2D &position, const CMouseButton &button,
              const uint &count=1, const CEventModifier &modifier=CMODIFIER_NONE) :
   CEvent(), position_(position), button_(button),
   count_(count), modifier_(modifier) {
  }

  CMouseEvent(const CMouseEvent &event) :
   CEvent(event), position_(event.position_), button_(event.button_),
   count_(event.count_), modifier_(event.modifier_) {
  }

  const CMouseEvent &operator=(const CMouseEvent &event) {
    position_ = event.position_;
    button_   = event.button_;
    count_    = event.count_;
    modifier_ = event.modifier_;

    return *this;
  }

  const CIPoint2D      &getPosition() const { return position_; }
  const CMouseButton   &getButton  () const { return button_  ; }
  const uint           &getCount   () const { return count_   ; }
  const CEventModifier &getModifier() const { return modifier_; }

  int getX() const { return position_.x; }
  int getY() const { return position_.y; }

  bool isButton(CMouseButton button) const { return button_ == button; }

  bool isShiftKey  () const { return (modifier_ & CMODIFIER_SHIFT  ); }
  bool isControlKey() const { return (modifier_ & CMODIFIER_CONTROL); }
  bool isAltKey    () const { return (modifier_ & CMODIFIER_ALT    ); }
  bool isMetaKey   () const { return (modifier_ & CMODIFIER_META   ); }

  void setPosition(const CIPoint2D      &position) { position_ = position; }
  void setButton  (const CMouseButton   &button  ) { button_   = button  ; }
  void setCount   (const uint           &count   ) { count_    = count   ; }
  void setModifier(const CEventModifier &modifier) { modifier_ = modifier; }

  void setX(int x) { position_.x = x; }
  void setY(int y) { position_.y = y; }

 private:
  CIPoint2D      position_;
  CMouseButton   button_;
  uint           count_;
  CEventModifier modifier_;
};

class CKeyEvent : public CEvent {
 public:
  CKeyEvent() :
   position_(0,0), type_(CKEY_TYPE_NUL), text_(""), modifier_(CMODIFIER_NONE) {
  }

  CKeyEvent(const CIPoint2D &position, const CKeyType &type, const std::string &text,
            const CEventModifier &modifier=CMODIFIER_NONE) :
   position_(position), type_(type), text_(text), modifier_(modifier) {
  }

  CKeyEvent(const CKeyEvent &event) :
   CEvent(), position_(event.position_), type_(event.type_),
   text_(event.text_), modifier_(event.modifier_) {
  }

  const CKeyEvent &operator=(const CKeyEvent &event) {
    position_ = event.position_;
    type_     = event.type_;
    text_     = event.text_;
    modifier_ = event.modifier_;

    return *this;
  }

  const CIPoint2D      &getPosition() const { return position_; }
  const CKeyType       &getType    () const { return type_    ; }
  const std::string    &getText    () const { return text_    ; }
  const CEventModifier &getModifier() const { return modifier_; }

  int getX() const { return position_.x; }
  int getY() const { return position_.y; }

  bool isShiftKey  () const { return (modifier_ & CMODIFIER_SHIFT  ); }
  bool isControlKey() const { return (modifier_ & CMODIFIER_CONTROL); }
  bool isAltKey    () const { return (modifier_ & CMODIFIER_ALT    ); }
  bool isMetaKey   () const { return (modifier_ & CMODIFIER_META   ); }

  void setPosition(const CIPoint2D      &position) { position_ = position; }
  void setType    (const CKeyType       &type    ) { type_     = type    ; }
  void setText    (const std::string    &text    ) { text_     = text    ; }
  void setModifier(const CEventModifier &modifier) { modifier_ = modifier; }

  void setX(int x) { position_.x = x; }
  void setY(int y) { position_.y = y; }

  bool isType(const CKeyType &type) const { return type == type_; }

  const std::string &getEscapeText() const;
  static const std::string &getEscapeText(CKeyType type);

  const std::string &getEscapeTextCSI() const;
  static const std::string &getEscapeTextCSI(CKeyType type);

  const std::string &getEscapeTextSS3() const;
  static const std::string &getEscapeTextSS3(CKeyType type);

  void print(std::ostream &os) const {
    os << position_ << " " << int(type_) << " " <<
          text_ << " " << int(modifier_);
  }

  friend std::ostream &operator<<(std::ostream &os, const CKeyEvent &event) {
    event.print(os);

    return os;
  }

 private:
  CIPoint2D      position_;
  CKeyType       type_;
  std::string    text_;
  CEventModifier modifier_;
};

class CEventAdapter {
 public:
  CEventAdapter() { }

  virtual ~CEventAdapter() { }

  virtual bool buttonPressEvent  (const CMouseEvent &) { return false; }
  virtual bool buttonMotionEvent (const CMouseEvent &) { return false; }
  virtual bool buttonReleaseEvent(const CMouseEvent &) { return false; }

  virtual bool keyPressEvent  (const CKeyEvent &) { return false; }
  virtual bool keyReleaseEvent(const CKeyEvent &) { return false; }

  virtual bool pointerMotionEvent(const CMouseEvent &) { return false; }

  virtual bool exposeEvent() { return false; }
  virtual bool resizeEvent() { return false; }

  virtual bool enterEvent() { return false; }
  virtual bool leaveEvent() { return false; }

  virtual bool closeEvent() { return false; }

  virtual bool visibilityEvent(bool) { return false; }

  virtual bool idleEvent() { return false; }

  virtual bool hideEvent() { return false; }
  virtual bool showEvent() { return false; }

  virtual bool clientMessageEvent(void *, const char *) { return false; }
};

#endif
