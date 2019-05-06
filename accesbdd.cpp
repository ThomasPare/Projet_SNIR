/**
  @file accesbdd.cpp
  @brief implémentation de la classe accesbdd
  @version 1.0
  @author LEVILLAIN Dimitri
  @date 25/03/2019
  @details Classe gérant les requetes SQL
  */

#include "accesbdd.h"
/**
 * @brief AccesBdd::AccesBdd
 * @details Constructeur
 */
AccesBdd::AccesBdd()
{

}
/**
 * @brief AccesBdd::ConnexionBdd
 * @param adresse
 * @param port
 * @param nomBDD
 * @param utilisateur
 * @param mdp
 * @return
 * @details COnnexion a la base de données
 */
bool AccesBdd::ConnexionBdd(QString adresse, int port, QString nomBDD, QString utilisateur, QString mdp)
{
    bool reussite = false;

    mySqlBdd = QSqlDatabase::addDatabase("QMYSQL");
    mySqlBdd.setPort(port);
    mySqlBdd.setHostName(adresse);
    mySqlBdd.setDatabaseName(nomBDD);

    if(mySqlBdd.open(utilisateur, mdp))
    {
        reussite = true;
    }
    return reussite;
}
/**
 * @brief AccesBdd::VerifierConnexion
 * @return
 * @details Verifie si on est correctement connecté à la base de données
 */
bool AccesBdd::VerifierConnexion()
{
    bool valide = true;

    if(!mySqlBdd.isOpen())
    {
        qDebug()<<"pb acces bdd" << mySqlBdd.lastError();
        valide = false;
    }
    return valide;
}
/**
 * @brief AccesBdd::ObtenirNomAgentsNomRondesDate
 * @details Requete SQL permetant de récupérer le nom des rondes, les agents et les dates
 */
/*void AccesBdd::ObtenirNomAgentsNomRondesDate()
{
    int i = 1;

    if(VerifierConnexion())
    {
        QSqlQuery requete(mySqlBdd);

        if(requete.exec("SELECT MAX(numeroRonde) From historiquepointeau;"))
        {
            requete.next();
            int numeroRondeMax = requete.value("MAX(numeroRonde)").toInt();
            qDebug()<<"Nombre de ronde : "<<numeroRondeMax;
            while(i<=numeroRondeMax){
                requete.prepare("SELECT nomrondes,ordrePointeau, agents.nom, historiquepointeau.date, idRonde, historiquepointeau.idAgent FROM mainscourantes, historiquepointeau,agents,rondes WHERE numeroRonde=:idr AND historiquepointeau.idAgent = agents.idAgent AND historiquepointeau.idRonde = rondes.idrondes;");
                requete.bindValue(":idr",i);
                if(requete.exec())
                {
                    requete.next();
                    QString nomRonde = requete.value("nomrondes").toString();
                    listeNomRonde.append(nomRonde);
                    QString nomAgents = requete.value("agents.nom").toString();
                    listeNomAgent.append(nomAgents);
                    QDateTime date = requete.value("historiquepointeau.date").toDateTime();
                    listeDate.append(date);
                    int idRonde = requete.value("idRonde").toInt();
                    listeIdRonde.append(idRonde);
                    int idAgent = requete.value("historiquepointeau.idAgent").toInt();
                    listeIdAgent.append(idAgent);
                }
                else
                {
                    qDebug() << "Erreur lors de la requête \"SELECT nomrondes, agents.nom, historiquepointeau.date, idRonde, historiquepointeau.idAgent FROM mainscourantes, historiquepointeau,agents,rondes WHERE numeroRonde=:idr AND historiquepointeau.idAgent = agents.idAgent AND historiquepointeau.idRonde = rondes.idrondes;\" : " + requete.lastError().text();
                }
                i++;
            }
        }
        else
        {
            qDebug() << "Erreur lors de la requête \"SELECT MAX(numeroRonde) From historiquepointeau;\" : " + requete.lastError().text();
        }
    }
    //qDebug()<<"liste : "<<liste;
}*/

void AccesBdd::ObtenirNomAgentsNomRondesDate()
{
    int i = 1;

    if(VerifierConnexion())
    {
        QSqlQuery requete(mySqlBdd);
        requete.prepare("SELECT ordrePointeau, historiquepointeau.date, idRonde, historiquepointeau.idAgent,rondes.nomrondes,agents.nom FROM historiquepointeau , agents , rondes WHERE historiquepointeau.idAgent=agents.idAgent AND historiquepointeau.idRonde=rondes.idrondes AND ordrePointeau=1;");
        if(requete.exec())
        {
            while(requete.next())
            {
                QString nomRonde = requete.value("nomrondes").toString();
                listeNomRonde.append(nomRonde);
                QString nomAgents = requete.value("agents.nom").toString();
                listeNomAgent.append(nomAgents);
                QDateTime date = requete.value("historiquepointeau.date").toDateTime();
                listeDate.append(date);
                int idRonde = requete.value("idRonde").toInt();
                listeIdRonde.append(idRonde);
                int idAgent = requete.value("historiquepointeau.idAgent").toInt();
                listeIdAgent.append(idAgent);
            }
        }
        else
        {
            qDebug() << "Erreur lors de la requête \"SELECT nomrondes, agents.nom, historiquepointeau.date, idRonde, historiquepointeau.idAgent FROM mainscourantes, historiquepointeau,agents,rondes WHERE numeroRonde=:idr AND historiquepointeau.idAgent = agents.idAgent AND historiquepointeau.idRonde = rondes.idrondes;\" : " + requete.lastError().text();
        }
        i++;
    }
    //qDebug()<<"liste : "<<liste;
}
/**
 * @brief AccesBdd::ObtenirIdPointeauDate
 * @param idAgent
 * @param idRonde
 * @details Effectue une requete sql permettant de recuperer la date et les idPointeaux en fonction de l'idAgent et de l'idRonde
 */
void AccesBdd::ObtenirIdPointeauDate(int idAgent, int idRonde)
{
    listeDate.clear();
    listeIdPointeaux.clear();
    qDebug()<<"idRonde : "<<idRonde;
    qDebug()<<"idAgent : "<<idAgent;

    if(VerifierConnexion())
    {
        QSqlQuery requete(mySqlBdd);

        requete.prepare("SELECT idPointeau, date FROM historiquepointeau WHERE idAgent = :ida AND idRonde = :idb;");
        requete.bindValue(":ida",idAgent);
        requete.bindValue(":idb",idRonde);
        if(requete.exec())
        {
            while (requete.next())
            {
                QDateTime date = requete.value("date").toDateTime();
                listeDatePointeau.append(date);
                int idPointeau = requete.value("idPointeau").toInt();
                listeIdPointeaux.append(idPointeau);
            }
        }
        else
        {
            qDebug() << "Erreur lors de la requête \"SELECT idPointeau, date FROM historiquepointeau WHERE idAgent = :ida AND idRonde = :idb;\" : " + requete.lastError().text();
        }
    }
}

/**
 * @brief AccesBdd::ObtenirListeNomAgent
 * @return
 */
QList<QString> AccesBdd::ObtenirListeNomAgent()
{
    return listeNomAgent;
}
/**
 * @brief AccesBdd::ObtenirListeNomRonde
 * @return
 */
QList<QString> AccesBdd::ObtenirListeNomRonde()
{
    return listeNomRonde;
}
/**
 * @brief AccesBdd::ObtenirListeDate
 * @return
 */
QList<QDateTime> AccesBdd::ObtenirListeDate()
{
    return listeDate;
}
/**
 * @brief AccesBdd::ObtenirIdRonde
 * @return
 */
QList<int> AccesBdd::ObtenirListeIdRonde()
{
    return listeIdRonde;
}
/**
 * @brief AccesBdd::ObtenirIdAgent
 * @return
 */
QList<int> AccesBdd::ObtenirListeIdAgent()
{
    return listeIdAgent;
}
/**
 * @brief AccesBdd::ObtenirListeDatePointeau
 * @return
 */
QList<QDateTime> AccesBdd::ObtenirListeDatePointeau()
{
    return listeDatePointeau;
}
/**
 * @brief AccesBdd::ObtenirListeIdPointeaux
 * @return
 */
QList<int> AccesBdd::ObtenirListeIdPointeaux()
{
    return listeIdPointeaux;
}

QList<QString> AccesBdd::ObtenirListeNomAbonnes()
{
    return listeNomAbonnes;
}

QList<int> AccesBdd::ObtenirListeIdAbonnes()
{
    return listeIdAbonnes;
}

void AccesBdd::ObtenirNomRondes()
{
    QString nomRonde;
    int idRonde;

    listeNomRonde.clear();
    listeIdRonde.clear();

    QSqlQuery requete(mySqlBdd);

    requete.prepare("SELECT nomrondes,idrondes FROM rondes;");
    if (!requete.exec()){
        qDebug()<<"pb requete "<<requete.lastError();
    }

    while(requete.next())
    {
        nomRonde=requete.value("nomrondes").toString();
        idRonde=requete.value("idrondes").toInt();
        listeNomRonde.append(nomRonde);
        listeIdRonde.append(idRonde);
    }
}

void AccesBdd::ObtenirNomAbonnes()
{
    QString nomAbonnes;
    QString prenomAbonnes;
    int idAbonnes;

    listeNomAbonnes.clear();
    listeIdAbonnes.clear();

    QSqlQuery requete(mySqlBdd);

    requete.prepare("select abonnes_nom,abonnes_prenom,abonnes_id from Liste_abonnes;");
    if (!requete.exec()){
        qDebug()<<"pb requete "<<requete.lastError();
    }

    while(requete.next())
    {
        nomAbonnes=requete.value("abonnes_nom").toString();
        prenomAbonnes=requete.value("abonnes_prenom").toString();
        idAbonnes=requete.value("abonnes_id").toInt();
        PrenomNomAbonne = prenomAbonnes + " " + nomAbonnes;
        listeNomAbonnes.append(PrenomNomAbonne);
        listeIdAbonnes.append(idAbonnes);
    }
}

QString AccesBdd::comboBoxAbonnesToQlist(QString prenom, QString nom)
{
    QString emailAbonnes;

    QSqlQuery requete(mySqlBdd);

    requete.prepare("SELECT abonnes_email FROM Liste_abonnes WHERE abonnes_nom= :idr1 AND abonnes_prenom= :idr2;" );
    requete.bindValue(":idr1",nom);
    requete.bindValue(":idr2",prenom);
    if(requete.exec())
    {
        while(requete.next())
        {
            emailAbonnes = requete.value("abonnes_email").toString();
        }
    }
    return emailAbonnes;
}

QString AccesBdd::QListToComboBoxAbonnes(QString email)
{
    QString PrenomNomAbonnes;

    QSqlQuery requete(mySqlBdd);

    requete.prepare("SELECT abonnes_nom, abonnes_prenom FROM Liste_abonnes WHERE abonnes_email= :idr;");
    requete.bindValue(":idr", email);
    if(requete.exec())
    {
        while(requete.next())
        {
            PrenomNomAbonnes = requete.value("abonnes_prenom").toString() + " " + requete.value("abonnes_nom").toString();
            qDebug()<<PrenomNomAbonnes ;
        }
    }
    return PrenomNomAbonnes;
}

void AccesBdd::SauvegardeMail(QString ronde, QString email)
{

    qDebug()<< email;

    QSqlQuery requeteMySQL(mySqlBdd);

    //prepare la requete pour sauvegarder l'email des abonnés et le nom du rapport sur la base de données des abonnés
    requeteMySQL.prepare("INSERT INTO Sauvegarde_pour_mail(Sauvegarde_nomRonde, Sauvegarde_EmailAbonne) VALUES (:nomRonde, :EmailAbonne)");

    requeteMySQL.bindValue(":nomRonde", ronde);
    requeteMySQL.bindValue(":EmailAbonne", email);

    if(!requeteMySQL.exec())
    {
        qDebug() << "Erreur lors de la copie de : " << email << "; erreur : " << mySqlBdd.lastError();
        qDebug() << "\n";

    }
    else
    {

        qDebug() << "Copie de l'abonne : " << email << " réussi " ;
        qDebug() << "\n";
    }

}






