#include <QWidget>

#include <string>

class CQPointEdit;

enum CQIllustratorPreferenceType {
  CQILLUSTRATOR_PREFERENCE_STRING_TYPE,
  CQILLUSTRATOR_PREFERENCE_INTEGER_TYPE,
  CQILLUSTRATOR_PREFERENCE_REAL_TYPE
};

class CQIllustrator;

class CQIllustratorPreferenceMgr {
 public:
  CQIllustratorPreferenceMgr(CQIllustrator *illustrator);

  void addStringPreference (const std::string &name, const std::string &def_value="");
  void addIntegerPreference(const std::string &name, int def_value=0);
  void addRealPreference   (const std::string &name, double def_value=0.0);
};

union CQIllustratorPreferenceValue {
  int     integer;
  double  real;
  char   *string;
};

class CQIllustratorPreference {
 public:
  CQIllustratorPreference() { }

 private:
  std::string                  name;
//CQIllustratorPreferenceType  type;
//CQIllustratorPreferenceValue value;
};

class CQIllustratorPreferenceDock : public QWidget {
  Q_OBJECT

 public:
  CQIllustratorPreferenceDock(CQIllustrator *illustrator);

 private slots:
  void syncFillBBox();
  void updateFillBBox();

 private:
  CQIllustrator *illustrator_;
  CQPointEdit   *fullMin_;
  CQPointEdit   *fullMax_;
};
