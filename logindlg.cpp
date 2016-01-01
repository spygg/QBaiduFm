#include "logindlg.h"
#include "ui_logindlg.h"
#include <QSettings>
#include <QDesktopWidget>

//调用刚才的函数把获取的网页内容储存为一个qstring的变量
const QString tokenUrl = "https://passport.baidu.com/v2/api/?getapi&class=login&tpl=mn&tangram=true";

LoginDlg::LoginDlg(QWidget *parent, QNetworkCookieJar *cookie) :
    QFrame(parent),
    ui(new Ui::LoginDlg)
{
    ui->setupUi(this);
    ui->verifywidget->hide();

    m_iLoginError = USER_PASSWD_ERROR;

    if(!cookie)
    {
        qDebug() << "传入cookie失败!";
        return;
    }
    m_cookJar = cookie;

    int iCheck = 0;
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "QBaiduFm", "QBaiduFm");
    settings.beginGroup("User");
    iCheck = settings.value("RememberMe").toInt();
    m_userName = settings.value("UserName").toString();
    m_passWd = settings.value("PassWd").toString();

//    if(iCheck)
//    {
//        logIn();
//    }
    settings.endGroup();

}

LoginDlg::~LoginDlg()
{
    delete ui;
}

int LoginDlg::logIn()
{
    getWebResponse("http://www.baidu.com");
    getWebResponse("https://passport.baidu.com/v2/api/?login");
    QString url = "https://passport.baidu.com/v2/api/?login";
    QByteArray ba;

    ba.append("ppui_logintime=");
    ba.append(QString::number(getRandNum(), 10));
    ba.append("&charset=utf-8");
    ba.append("&codestring=");
    ba.append("&token=");
    ba.append( getToken(tokenUrl));
    ba.append("&isPhone=false");
    ba.append("&index=0");
    ba.append("&u=");
    ba.append("&safeflg=0");
    ba.append("&staticpage=http%3A%2F%2Fwww.baidu.com%2Fcache%2Fuser%2Fhtml%2Fjump.html");
    ba.append("&loginType=1");
    ba.append("&tpl=mn");
    ba.append("&callback=parent.bdPass.api.login._postCallback");
    ba.append("&username=");
    ba.append(m_userName);
    ba.append("&password=");
    ba.append(m_passWd);
    ba.append("&verifycode=");
    ba.append("&mem_pass=on");

    QString postRet = postWebResponse(url, ba);

    qDebug() << postRet;
    if(postRet.contains("error=0"))
    {
        qDebug() << "登录成功" << m_userName;
        m_iLoginError = LONIN_SUCCESS_ERROR;
        emit loginSuccess(m_userName);
    }
    else if(postRet.contains("error=2"))
    {
        qDebug() << "用户名密码错误";
        m_iLoginError = USER_PASSWD_ERROR;
        emit loginSuccess("登录失败");

    }
    else if(postRet.contains("error=5"))
    {
        qDebug() << "输入验证码";
        m_iLoginError = VERIFY_PICTURE_ERROR;
        emit loginSuccess("登录失败");
    }

    return m_iLoginError;
}

void LoginDlg::on_toolButtonLogIn_clicked()
{
    if(m_iLoginError == LONIN_SUCCESS_ERROR)
    {
        emit loginSuccess(m_userName);
        hide();
        return;
    }

    QString userName = ui->lineEditUserName->text();
    QString passWd = ui->lineEditPasswd->text();
    Qt::CheckState check = ui->checkBoxRememberMe->checkState();

    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "QBaiduFm", "QBaiduFm");
    settings.beginGroup("User");
    settings.setValue("RememberMe", check);
    if(check)
    {
        settings.setValue("UserName", userName);
        settings.setValue("PassWd", passWd);
    }
    settings.endGroup();

    int iLoginSuccess = logIn();
    if(iLoginSuccess == LONIN_SUCCESS_ERROR)
    {
        hide();
    }
    else if(iLoginSuccess == USER_PASSWD_ERROR)
    {
        showDlg(USER_PASSWD_ERROR);
    }
    else if(iLoginSuccess == VERIFY_PICTURE_ERROR)
    {
        ui->verifywidget->show();
        showDlg(VERIFY_PICTURE_ERROR);
    }
}

void LoginDlg::on_toolButtonCancel_clicked()
{
    hide();
}

void LoginDlg::showDlg(int iShowState)
{
    if(iShowState == USER_PASSWD_ERROR)
    {
        ui->lineEditUserName->clear();
        ui->lineEditPasswd->clear();
        ui->lineEditUserName->setPlaceholderText("用户名或密码错误");
        ui->lineEditPasswd->setPlaceholderText("用户名或密码错误");
    }

    if(iShowState == VERIFY_PICTURE_ERROR)
    {
        ui->lineEditUserName->setPlaceholderText("请输入用户名");
        ui->lineEditPasswd->setPlaceholderText("请输入密码");
    }

    int iCheck = 0;
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "QBaiduFm", "QBaiduFm");
    settings.beginGroup("User");
    iCheck = settings.value("RememberMe").toInt();
    ui->checkBoxRememberMe->setChecked(iCheck);
    m_userName = settings.value("UserName").toString();
    m_passWd = settings.value("PassWd").toString();

    if(iCheck)
    {
        ui->lineEditUserName->setText(m_userName);
        ui->lineEditPasswd->setText(m_passWd);
    }

    settings.endGroup();
    show();
}

int LoginDlg::getRandNum()
{
    int irandNum = qrand() %(99999-10000+1)+10000;
    return irandNum;
}

QString LoginDlg::getWebResponse(QString url)
{
    QNetworkAccessManager *manager = new QNetworkAccessManager;
    QNetworkRequest request;
    QByteArray responseData;

    manager->setCookieJar(m_cookJar);
    //设置网址
    request.setUrl(QUrl(url));
    QNetworkReply *reply = manager->get(request);

    QEventLoop loop;
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    responseData = reply->readAll();

    reply->deleteLater();

    //qDebug() << responseData;
    return QString(responseData); //返回取到的源码字符串
}

QString LoginDlg::getToken(QString url)
{
    QString response;
    response = getWebResponse(url);

    QRegExp rx;
    rx.setPattern("token='([0-9 | a-f]{1,})");

    //下面就是匹配字符串（其实用正则更好）
    int start = rx.indexIn(response);
    int end = rx.matchedLength();


    QString token = response.mid(start, end);

    token = token.mid(7, token.size());

//    qDebug() << token;
    return token; //返回获取的token
}


QString LoginDlg::postWebResponse(QString url, QByteArray postData)
{
    //qDebug() << postData;
    QNetworkAccessManager *manager = new QNetworkAccessManager;
    QNetworkRequest request;
    QByteArray responseData;

    manager->setCookieJar(m_cookJar);
    //设置网址
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setHeader(QNetworkRequest::ContentLengthHeader, postData.length());
    request.setUrl(QUrl(url));
    QNetworkReply *reply = manager->post(request, postData);

    QEventLoop loop;
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    responseData = reply->readAll();


//    foreach(QNetworkCookie cookie, manager->cookieJar()->cookiesForUrl(QUrl(url)))
//    {
//        qDebug() << cookie.value();
//    }

    reply->deleteLater();

//    qDebug() << responseData;
    return QString(responseData); //返回取到的源码字符串
}

