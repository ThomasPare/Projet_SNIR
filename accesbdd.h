#ifndef ACCESBDD_H
#define ACCESBDD_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDateTime>


class AccesBdd
{
public:
    AccesBdd();

    bool ConnexionBdd(QString adresse, int port, QString nomBDD, QString utilisateur, QString mdp);
    bool VerifierConnexion();
    void ObtenirNomAgentsNomRondesDate();
    void ObtenirIdPointeauDate(int idAgent, int idRonde);
    void ObtenirNomRondes();
    void ObtenirNomAbonnes();
    QString comboBoxAbonnesToQlist(QString prenom, QString nom);
    QString QListToComboBoxAbonnes(QString email);
    void SauvegardeMail(QString ronde, QString email);


    QList<QString> ObtenirListeNomAgent();
    QList<QString> ObtenirListeNomRonde();
    QList<QDateTime> ObtenirListeDate();
    QList<int> ObtenirListeIdRonde();
    QList<int> ObtenirListeIdAgent();

    QList<QDateTime> ObtenirListeDatePointeau();
    QList<int> ObtenirListeIdPointeaux();

    QList<QString> ObtenirListeNomAbonnes();
    QList<int> ObtenirListeIdAbonnes();

private:
    QSqlDatabase mySqlBdd;
    QList<QString> listeNomAgent;
    QList<QString> listeNomRonde;
    QList<QDateTime> listeDate;
    QList<int> listeIdRonde;
    QList<int> listeIdAgent;
    QString PrenomNomAbonne;
    QList<QString> listeNomAbonnes;
    QList<int> listeIdAbonnes;

    QList<QDateTime> listeDatePointeau;
    QList<int> listeIdPointeaux;
};

#endif // ACCESBDD_H
