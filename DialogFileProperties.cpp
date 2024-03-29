#include "DialogFileProperties.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QGroupBox>
#include <QListWidget>

#include <QDateTime>

//! [0]
DialogFileProperties::DialogFileProperties(const std::filesystem::path &fileName, QWidget *parent)
    : QDialog(parent)
{
  QFileInfo fileInfo(fileName);
  this->BuildFileProperties(fileInfo);
}

//! [0]
DialogFileProperties::DialogFileProperties(const QString &fileName, QWidget *parent)
    : QDialog(parent)
{
  QFileInfo fileInfo(fileName);
  this->BuildFileProperties(fileInfo);
}

void DialogFileProperties::BuildFileProperties(const QFileInfo &fileInfo)
{
  this->setWindowTitle(tr("File properties"));
  this->setSizeGripEnabled(true);

  tabWidget = new QTabWidget;
  tabWidget->addTab(new GeneralTab(fileInfo), tr("General"));
  tabWidget->addTab(new PermissionsTab(fileInfo), tr("Permissions"));
  tabWidget->addTab(new ApplicationsTab(fileInfo), tr("Applications"));
  //! [0]

  //! [1] //! [2]
  buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
  connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
  connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
  //! [2] //! [3]

  //! [4]
  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget(tabWidget);
  mainLayout->addWidget(buttonBox);
  this->setLayout(mainLayout);
} 

GeneralTab::GeneralTab(const QFileInfo &fileInfo, QWidget *parent)
    : QWidget(parent)
{
  QLabel *fileNameLabel = new QLabel(tr("File Name:"));
  QLineEdit *fileNameEdit = new QLineEdit(fileInfo.fileName());

  QLabel *pathLabel = new QLabel(tr("Path:"));
  QLabel *pathValueLabel = new QLabel(fileInfo.absoluteFilePath());
  pathValueLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);

  QLabel *sizeLabel = new QLabel(tr("Size:"));
  qlonglong size = fileInfo.size() / 1024;
  QLabel *sizeValueLabel = new QLabel(tr("%1 K").arg(size));
  sizeValueLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);

  QLabel *lastReadLabel = new QLabel(tr("Last Read:"));
  QLabel *lastReadValueLabel = new QLabel(fileInfo.lastRead().toString());
  lastReadValueLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);

  QLabel *lastModLabel = new QLabel(tr("Last Modified:"));
  QLabel *lastModValueLabel = new QLabel(fileInfo.lastModified().toString());
  lastModValueLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget(fileNameLabel);
  mainLayout->addWidget(fileNameEdit);
  mainLayout->addWidget(pathLabel);
  mainLayout->addWidget(pathValueLabel);
  mainLayout->addWidget(sizeLabel);
  mainLayout->addWidget(sizeValueLabel);
  mainLayout->addWidget(lastReadLabel);
  mainLayout->addWidget(lastReadValueLabel);
  mainLayout->addWidget(lastModLabel);
  mainLayout->addWidget(lastModValueLabel);
  mainLayout->addStretch(1);
  setLayout(mainLayout);
}
//! [6]

//! [7]
PermissionsTab::PermissionsTab(const QFileInfo &fileInfo, QWidget *parent)
    : QWidget(parent)
{
  QGroupBox *permissionsGroup = new QGroupBox(tr("Permissions"));

  QCheckBox *readable = new QCheckBox(tr("Readable"));
  if (fileInfo.isReadable())
    readable->setChecked(true);

  QCheckBox *writable = new QCheckBox(tr("Writable"));
  if (fileInfo.isWritable())
    writable->setChecked(true);

  QCheckBox *executable = new QCheckBox(tr("Executable"));
  if (fileInfo.isExecutable())
    executable->setChecked(true);

  QGroupBox *ownerGroup = new QGroupBox(tr("Ownership"));

  QLabel *ownerLabel = new QLabel(tr("Owner"));
  QLabel *ownerValueLabel = new QLabel(fileInfo.owner());
  ownerValueLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);

  QLabel *groupLabel = new QLabel(tr("Group"));
  QLabel *groupValueLabel = new QLabel(fileInfo.group());
  groupValueLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);

  QVBoxLayout *permissionsLayout = new QVBoxLayout;
  permissionsLayout->addWidget(readable);
  permissionsLayout->addWidget(writable);
  permissionsLayout->addWidget(executable);
  permissionsGroup->setLayout(permissionsLayout);

  QVBoxLayout *ownerLayout = new QVBoxLayout;
  ownerLayout->addWidget(ownerLabel);
  ownerLayout->addWidget(ownerValueLabel);
  ownerLayout->addWidget(groupLabel);
  ownerLayout->addWidget(groupValueLabel);
  ownerGroup->setLayout(ownerLayout);

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget(permissionsGroup);
  mainLayout->addWidget(ownerGroup);
  mainLayout->addStretch(1);
  setLayout(mainLayout);
}
//! [7]

//! [8]
ApplicationsTab::ApplicationsTab(const QFileInfo &fileInfo, QWidget *parent)
    : QWidget(parent)
{
  QLabel *topLabel = new QLabel(tr("Open with:"));

  QListWidget *applicationsListBox = new QListWidget;
  QStringList applications;

  for (int i = 1; i <= 30; ++i)
    applications.append(tr("Application %1").arg(i));
  applicationsListBox->insertItems(0, applications);

  QCheckBox *alwaysCheckBox;

  QString always(tr("Always use this application to open this type of file"));

  if (!fileInfo.suffix().isEmpty())
    always += (tr(" with the extension '%1'").arg(fileInfo.suffix()));
  alwaysCheckBox = new QCheckBox(always);

  QVBoxLayout *layout = new QVBoxLayout;
  layout->addWidget(topLabel);
  layout->addWidget(applicationsListBox);
  layout->addWidget(alwaysCheckBox);
  setLayout(layout);
}
//! [8]
