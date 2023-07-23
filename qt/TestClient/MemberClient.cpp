#include "MemberClient.h"
#include "MemberTcpClient.h"

MemberClient::MemberClient(const QString &crew, const QString &name, const QString &password, QObject *parent)
	: BlrClient(Member, crew, name, password, parent)
{
}

TcpClient *MemberClient::createTcpClient()
{
	return new MemberTcpClient(m_handler.crew, m_handler.name, m_handler.password);
}

void MemberClient::createConnections()
{
	BlrClient::createConnections();
	//MemberTcpClient *tcpclient = qobject_cast<MemberTcpClient *>(m_tcpclient);
}
