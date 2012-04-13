#include "HelpBrowser.h"
#include <QHBoxLayout>
#include <QTextBrowser>
#include <QPushButton>
#include <QVBoxLayout>
#include <QStringList>
#include <QDir>
#include <QDebug>

HelpBrowser::HelpBrowser(const QString &path, const QString &page)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setAttribute(Qt::WA_GroupLeader);
    textBrowser = new QTextBrowser;
    homeButton = new QPushButton(tr("&Home"));
    backButton = new QPushButton(tr("&Back"));
    closeButton = new QPushButton(tr("Close"));
    closeButton->setShortcut(tr("Esc"));
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(homeButton);
    buttonLayout->addWidget(backButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeButton);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(textBrowser);
    setLayout(mainLayout);
    connect(homeButton, SIGNAL(clicked()), textBrowser, SLOT(home()));
    connect (backButton, SIGNAL(clicked()), textBrowser, SLOT(backward()));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(textBrowser, SIGNAL(sourceChanged(const QUrl &)),
    this, SLOT(updateWindowTitle()));
    QStringList a;
    a.append( path );
    a.append( ":/images");
    textBrowser->setSearchPaths( a );
    textBrowser->setSource(page);
}

void HelpBrowser::showPage(const QString &page)
{
    QString path = QDir("media/doc").absolutePath();
    HelpBrowser *browser = new HelpBrowser(path, page);
    browser->resize(500, 400);
    browser->show();
}

void HelpBrowser::updateWindowTitle()
{
    setWindowTitle(tr("Help: %1").arg(textBrowser->documentTitle()));
}

