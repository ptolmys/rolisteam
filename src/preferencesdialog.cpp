/*************************************************************************
 *     Copyright (C) 2010 by Joseph Boudou                               *
 *                                                                       *
 *     http://www.rolisteam.org/                                         *
 *                                                                       *
 *   rolisteam is free software; you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published   *
 *   by the Free Software Foundation; either version 2 of the License,   *
 *   or (at your option) any later version.                              *
 *                                                                       *
 *   This program is distributed in the hope that it will be useful,     *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of      *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the       *
 *   GNU General Public License for more details.                        *
 *                                                                       *
 *   You should have received a copy of the GNU General Public License   *
 *   along with this program; if not, write to the                       *
 *   Free Software Foundation, Inc.,                                     *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.           *
 *************************************************************************/

#include "preferencesdialog.h"
#include "constantesGlobales.h"
#include "variablesGlobales.h"
#include "initialisation.h"

#include <QFileDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QPushButton>

/**************
 * DirChooser *
 **************/

DirChooser::DirChooser(QWidget * parent)
 : QWidget(parent)
{
    // Childrens
    m_lineEdit = new QLineEdit(this);
    m_lineEdit->setText(QDir::homePath());
    QPushButton * button = new QPushButton(tr("..."), this);

    // Layout
    QHBoxLayout * layout = new QHBoxLayout;
    layout->addWidget(m_lineEdit, 1);
    layout->addWidget(button, 0);
    setLayout(layout);

    // Connections
    connect(button, SIGNAL(pressed()), this, SLOT(browse()));

    // Misc
    button->setMaximumWidth(28);
    setContentsMargins(0,0,0,0);
    layout->setContentsMargins(0,0,0,0);
}

DirChooser::~DirChooser()
{
    // QObject should do it right for us already.
}

void DirChooser::setDirName(const QString & dirName)
{
    m_lineEdit->setText(dirName);
}

QString DirChooser::dirName() const
{
    return m_lineEdit->text();
}

void DirChooser::browse()
{
    QString result = QFileDialog::getExistingDirectory(this,
            tr("Select directory"),
            m_lineEdit->text(),
            QFileDialog::ShowDirsOnly );
    if (!result.isEmpty())
    {
        m_lineEdit->setText(result);
    }
}


/*********************
 * PreferencesDialog *
 *********************/

PreferencesDialog::PreferencesDialog(QWidget * parent, Qt::WindowFlags f)
 : QDialog(parent, f)
{

    m_preferences = PreferencesManager::getInstance();
    // Children
#ifndef NULL_PLAYER
    m_gmMusicDir     = new DirChooser;
    m_playerMusicDir = new DirChooser;
#endif
    m_picturesDir    = new DirChooser;
    m_mapsDir        = new DirChooser;
    m_sessionsDir    = new DirChooser;
    m_notesDir       = new DirChooser;
    m_chatsDir       = new DirChooser;


    QHBoxLayout * layoutH = new QHBoxLayout;
    layoutH->addWidget(new QLabel(tr("Check update at start up:")));
    m_checkUpdateAtStartUp = new QCheckBox();
    layoutH->addWidget(m_checkUpdateAtStartUp);
    // Form layout
    QFormLayout * formLayout = new QFormLayout;
#ifndef NULL_PLAYER
    formLayout->addRow(tr("Music directory (GM) : "), m_gmMusicDir);
    formLayout->addRow(tr("Music directory (Player) : "), m_playerMusicDir);
#endif
    formLayout->addRow(tr("Picture directory : "), m_picturesDir);
    formLayout->addRow(tr("Map directory : "), m_mapsDir);
    formLayout->addRow(tr("Scenario directory : "), m_sessionsDir);
    formLayout->addRow(tr("Minutes directory : "), m_notesDir);
    formLayout->addRow(tr("Chat conversation : "), m_chatsDir);

    // Buttons
    QDialogButtonBox * buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    // Main layout
    QVBoxLayout * layout = new QVBoxLayout;
    layout->addLayout(layoutH);
    layout->addLayout(formLayout);
    layout->addWidget(buttonBox);

    setLayout(layout);

    // Connections
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    connect(this, SIGNAL(accepted()), this, SLOT(save()));

    // Misc
    setSizeGripEnabled(true);
    setWindowTitle(QString("%1 - %2").arg(m_preferences->value("Application_Name","rolisteam").toString(),tr("Preferences")));
    setWindowModality(Qt::ApplicationModal);
}

PreferencesDialog::~PreferencesDialog()
{
    // QObject should do it right for us allready.
}

void PreferencesDialog::show()
{
    load();
    QDialog::show();
}

void PreferencesDialog::load()
{
#ifndef NULL_PLAYER
    m_gmMusicDir->setDirName(m_preferences->value("MusicDirectoryGM",QDir::homePath()).toString());
    m_playerMusicDir->setDirName(m_preferences->value("MusicDirectoryPlayer",QDir::homePath()).toString());
#endif
    m_picturesDir->setDirName(m_preferences->value("ImageDirectory",QDir::homePath()).toString());
    m_mapsDir->setDirName(m_preferences->value("MapDirectory",QDir::homePath()).toString());
    m_sessionsDir->setDirName(m_preferences->value("SessionDirectory",QDir::homePath()).toString());
    m_notesDir->setDirName(m_preferences->value("MinutesDirectory",QDir::homePath()).toString());
    m_chatsDir->setDirName(m_preferences->value("ChatDirectory",QDir::homePath()).toString());
    m_checkUpdateAtStartUp->setChecked(m_preferences->value("MainWindow_MustBeChecked",true).toBool());
}

void PreferencesDialog::save() const
{
#ifndef NULL_PLAYER
    m_preferences->registerValue("MusicDirectoryGM",m_gmMusicDir->dirName());
    m_preferences->registerValue("MusicDirectoryPlayer",m_gmMusicDir->dirName());
#endif
    m_preferences->registerValue("ImageDirectory",m_picturesDir->dirName());
    m_preferences->registerValue("MapDirectory",m_mapsDir->dirName());
    m_preferences->registerValue("SessionDirectory",m_sessionsDir->dirName());
    m_preferences->registerValue("MinutesDirectory",m_notesDir->dirName());
    m_preferences->registerValue("ChatDirectory",m_chatsDir->dirName());
    m_preferences->registerValue("MainWindow_MustBeChecked",m_checkUpdateAtStartUp->isChecked());
}
