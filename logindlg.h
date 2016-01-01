#ifndef LOGINDLG_H
#define LOGINDLG_H

#include <QFrame>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QEventLoop>
#include <QNetworkCookieJar>
#include <QNetworkCookie>



enum LOGIN_ERROR
{
    LONIN_SUCCESS_ERROR,
    USER_PASSWD_ERROR,
    VERIFY_PICTURE_ERROR
};

namespace Ui {
class LoginDlg;
}

class LoginDlg : public QFrame
{
    Q_OBJECT

public:
    explicit LoginDlg(QWidget *parent = 0, QNetworkCookieJar *cookie = NULL);
    ~LoginDlg();

private slots:
    void on_toolButtonLogIn_clicked();

    void on_toolButtonCancel_clicked();

private:
    Ui::LoginDlg *ui;

public:
    void showDlg(int iShowState = 0);
    int logIn();

private:
    int getRandNum();
    QString getWebResponse(QString url);
    QString getToken(QString url);
    QString postWebResponse(QString url, QByteArray postData);

private:
    QNetworkCookieJar *m_cookJar;

    int m_iLoginError;
    QString m_userName;
    QString m_passWd;

signals:
    void loginSuccess(QString userName);
};

#endif // LOGINDLG_H
