/**
  @file envoiemail.cpp
  @brief implémentation de la classe envoieMail
  @version 1.0
  @author Thomas PARÉ
  @date 19/03/2019
  @details Classe gérant l'interface permettant de choisir quel rapport envoyé et à qui l'envoyer
  */

#include "envoiemail.h"
#include "ui_envoiemail.h"

/**
  @brief EnvoieMail::EnvoieMail
  @details le constructeur permet de se connecter à la base de données de supervision et à la base de données des abonnés, il permet également de remplir les comboBox avec les infos de la base de données.
  @author Thomas PARÉ
  */
EnvoieMail::EnvoieMail(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EnvoieMail)
{
    ui->setupUi(this);

    ui->comboBoxListeAbonnees->addItem("Choisir abonné");

    QString nomFichierIni="ConnexionBdd.ini";
    QFileInfo testFichier(nomFichierIni);

    if(testFichier.exists() && testFichier.isFile())
    {
        QSettings paramSocket(nomFichierIni,QSettings::IniFormat);

        QString adresseIpSupervision=paramSocket.value("CONFIG_BDD_SUPERVISION/ip").toString();
        QString nomBaseSupervision=paramSocket.value("CONFIG_BDD_SUPERVISION/nom").toString();
        QString userSupervision=paramSocket.value("CONFIG_BDD_SUPERVISION/user").toString();
        QString mdpSupervision=paramSocket.value("CONFIG_BDD_SUPERVISION/mdp").toString();

        QString adresseIpAbonnes=paramSocket.value("CONFIG_BDD_ABONNES/ip").toString();
        QString nomBaseAbonnes=paramSocket.value("CONFIG_BDD_ABONNES/nom").toString();
        QString userAbonnes=paramSocket.value("CONFIG_BDD_ABONNES/user").toString();
        QString mdpAbonnes=paramSocket.value("CONFIG_BDD_ABONNES/mdp").toString();

        //Connexion à la base de données des utilisateurs à l'adresse 172.18.58.5

        bdAgents = QSqlDatabase::addDatabase("QMYSQL");
        bdAgents.setHostName(adresseIpSupervision);
        bdAgents.setDatabaseName(nomBaseSupervision);
        bdAgents.setUserName(userSupervision);
        bdAgents.setPassword(mdpSupervision);
        //Ouverture de la base par QT
        if(!bdAgents.open())
        {
            qDebug()<<"pb acces bd"<<bdAgents.lastError();
        }
        else
        {
            qDebug()<<"acces bd agents ok";
        }
        //requete pour récuperer le nom des rondes et leur id
        QSqlQuery requeteAgents("select nom,idRonde from rondes;" );
        if (!requeteAgents.exec()){
            qDebug()<<"pb requete "<<requeteAgents.lastError();
        }
        //affichage des agents dans la comboBox

        while(requeteAgents.next())
        {
            nomRonde=requeteAgents.value("nom").toString();
            idRonde=requeteAgents.value("idRonde").toInt();
            ui->comboBoxRondeASelectionner->addItem(nomRonde,idRonde);
        }

        bdAgents.removeDatabase("QMYSQL");

        //Connexion à la base de données des utilisateurs à l'adresse 172.18.58.86

        bdAbonnes = QSqlDatabase::addDatabase("QMYSQL");
        bdAbonnes.setHostName(adresseIpAbonnes);
        bdAbonnes.setDatabaseName(nomBaseAbonnes);
        bdAbonnes.setUserName(userAbonnes);
        bdAbonnes.setPassword(mdpAbonnes);
        //Ouverture de la base par QT
        if(!bdAbonnes.open())
        {
            qDebug()<<"pb acces bd abonnes"<<bdAbonnes.lastError();
        }
        else
        {
            qDebug()<<"acces bd ok abonnes";
        }
        //requete pour récuperer le prénom et le nom des abonnés, leur email et leur id
        QSqlQuery requeteAbonnes("select abonnes_nom,abonnes_prenom,abonnes_id from Liste_abonnes;");
        if (!requeteAbonnes.exec()){
            qDebug()<<"pb requete "<<requeteAbonnes.lastError();
        }
        //affichage des agents dans la comboBox

        while(requeteAbonnes.next())
        {
            nomAbonnes=requeteAbonnes.value("abonnes_nom").toString();
            prenomAbonnes=requeteAbonnes.value("abonnes_prenom").toString();
            //emailAbonnes=requeteAbonnes.value("abonnes_email").toString();
            idAbonnes=requeteAbonnes.value("abonnes_id").toInt();
            ui->comboBoxListeAbonnees->addItem(nomAbonnes + " " + prenomAbonnes,idAbonnes);
        }

        nbAbonnesSelectionne = 0;
    }
    else
    {
        qDebug()<<"fichier ini non valide";
    }


}

EnvoieMail::~EnvoieMail()
{
    delete ui;
}

/**
  @brief EnvoieMail::MettreAJourDestinataireEmail
  @details cette méthode permet d'enregistrer les abonnés choisit sur la base de données des abonnés
  @author Thomas PARÉ
  */
void EnvoieMail::MettreAJourDestinataireEmail()
{
    for (int i=0; i<nbAbonnesSelectionne; i++)
    {
        qDebug() << ui->listWidget->item(i)->text();

        QSqlQuery requeteMySQL;

        //prepare la requete pour sauvegarder l'email des abonnés et le nom du rapport sur la base de données des abonnés
        requeteMySQL.prepare("INSERT INTO Sauvegarde_pour_mail(nomRonde, EmailAbonne) VALUES (:nomRonde, :EmailAbonne)");

        requeteMySQL.bindValue(":nomRonde", ui->comboBoxRondeASelectionner->currentText());
        requeteMySQL.bindValue(":EmailAbonne", ui->listWidget->item(i)->text());

        if(!requeteMySQL.exec())
        {
            qDebug() << "Erreur lors de la copie de : " << ui->listWidget->item(i)->text() << "; erreur : " << bdAbonnes.lastError();
            qDebug() << "\n";

        }
        else
        {
            for (int i=0; i<nbAbonnesSelectionne; i++)
            {
                qDebug() << "Copie de l'abonne : " << ui->listWidget->item(i)->text() << " réussi " ;
                qDebug() << "\n";
            }
        }
    }
}


/**
  @brief EnvoieMail::on_comboBoxListeAbonnees_activated
  @details cette méthode est appelé lorsque qu'un abonné est choisit dans la comboBox comboBoxListeAbonnees
  @author Thomas PARÉ
  */
void EnvoieMail::on_comboBoxListeAbonnees_activated(const QString &arg1)
{
    QSqlQuery requeteEmail;

    if(!(arg1 == "Choisir abonné"))
    {
        QList <QString> listeNomPrenom=arg1.split(" ");
        qDebug()<<"nom : "<<listeNomPrenom[0]<<" prenom : "<<listeNomPrenom[1];
        QString nom=listeNomPrenom[0];
        QString prenom=listeNomPrenom[1];
        requeteEmail.prepare("SELECT abonnes_email FROM Liste_abonnes WHERE abonnes_nom= :idr1 AND abonnes_prenom= :idr2;" );
        requeteEmail.bindValue(":idr1",nom);
        requeteEmail.bindValue(":idr2",prenom);
        if(requeteEmail.exec())
        {
            requeteEmail.next();
            emailAbonnes=requeteEmail.value("abonnes_email").toString();
            ui->listWidget->addItem(emailAbonnes);
            ui->comboBoxListeAbonnees->removeItem(ui->comboBoxListeAbonnees->currentIndex());
            nbAbonnesSelectionne ++;
            qDebug() << nbAbonnesSelectionne;
        }
    }
}

/**
  @brief EnvoieMail::on_pushButtonSupprimerAbonne_clicked
  @details cette méthode permet de supprimer un abonné du listWidget et de le remettre dans le comboBox
  @author Thomas PARÉ
  */
void EnvoieMail::on_pushButtonSupprimerAbonne_clicked()
{
    QSqlQuery requeteRetourComboBox;
    if(ui->listWidget->count()>0)
    {
        requeteRetourComboBox.prepare("SELECT abonnes_nom, abonnes_prenom FROM Liste_abonnes WHERE abonnes_email= :idr;");
        requeteRetourComboBox.bindValue(":idr",ui->listWidget->currentItem()->text());
        if(requeteRetourComboBox.exec())
        {
            requeteRetourComboBox.next();
            nomAbonnes=requeteRetourComboBox.value("abonnes_nom").toString();
            prenomAbonnes=requeteRetourComboBox.value("abonnes_prenom").toString();
            ui->comboBoxListeAbonnees->addItem(prenomAbonnes + " " + nomAbonnes);
            delete ui->listWidget->currentItem();
            nbAbonnesSelectionne -- ;
        }
    }
    qDebug() << nbAbonnesSelectionne;
}

/**
  @brief EnvoieMail::on_pushButtonValiderListe_clicked
  @details Quand on appuie sur le bouton "Valider liste", la méthode MettreAJourDestinataireEmail est appelé et un pop-up apparaît pour confirmer la validation
  @author Thomas PARÉ
  */
void EnvoieMail::on_pushButtonValiderListe_clicked()
{
    if(!(ui->listWidget->count() == 0))
    {
        MettreAJourDestinataireEmail();
        QMessageBox::information(this, "Validation", "Votre choix à bien été sauvegardé");
        for(int i=0; i<nbAbonnesSelectionne; i++)
        {
            ui->comboBoxListeAbonnees->addItem(ui->listWidget->item(i)->text());
        }
        ui->listWidget->clear();
        nbAbonnesSelectionne = 0;
    }else
    {
        QMessageBox::information(this, "Erreur", "Aucun abonné n'a été choisi, veuillez réessayer");
    }
}

void EnvoieMail::on_pushButtonModifierAdresses_clicked()
{
    ChangementsAdresses *fenetre = new ChangementsAdresses;
    fenetre->show();
}

void EnvoieMail::on_pushButtonRafraichirInterface_clicked()
{
    ui->comboBoxRondeASelectionner->clear();
    ui->comboBoxListeAbonnees->clear();
    ui->listWidget->clear();

    ui->comboBoxListeAbonnees->addItem("Choisir abonné");

    QString nomFichierIni="ConnexionBdd.ini";
    QFileInfo testFichier(nomFichierIni);

    if(testFichier.exists() && testFichier.isFile())
    {
        QSettings paramSocket(nomFichierIni,QSettings::IniFormat);

        QString adresseIpSupervision=paramSocket.value("CONFIG_BDD_SUPERVISION/ip").toString();
        QString nomBaseSupervision=paramSocket.value("CONFIG_BDD_SUPERVISION/nom").toString();
        QString userSupervision=paramSocket.value("CONFIG_BDD_SUPERVISION/user").toString();
        QString mdpSupervision=paramSocket.value("CONFIG_BDD_SUPERVISION/mdp").toString();

        QString adresseIpAbonnes=paramSocket.value("CONFIG_BDD_ABONNES/ip").toString();
        QString nomBaseAbonnes=paramSocket.value("CONFIG_BDD_ABONNES/nom").toString();
        QString userAbonnes=paramSocket.value("CONFIG_BDD_ABONNES/user").toString();
        QString mdpAbonnes=paramSocket.value("CONFIG_BDD_ABONNES/mdp").toString();

        //Connexion à la base de données des utilisateurs à l'adresse 172.18.58.5

        bdAgents = QSqlDatabase::addDatabase("QMYSQL");
        bdAgents.setHostName(adresseIpSupervision);
        bdAgents.setDatabaseName(nomBaseSupervision);
        bdAgents.setUserName(userSupervision);
        bdAgents.setPassword(mdpSupervision);
        //Ouverture de la base par QT
        if(!bdAgents.open())
        {
            qDebug()<<"pb acces bd"<<bdAgents.lastError();
        }
        else
        {
            qDebug()<<"acces bd agents ok";
        }
        //requete pour récuperer le nom des rondes et leur id
        QSqlQuery requeteAgents("select nom,idRonde from rondes;" );
        if (!requeteAgents.exec()){
            qDebug()<<"pb requete "<<requeteAgents.lastError();
        }
        //affichage des agents dans la comboBox

        while(requeteAgents.next())
        {
            nomRonde=requeteAgents.value("nom").toString();
            idRonde=requeteAgents.value("idRonde").toInt();
            ui->comboBoxRondeASelectionner->addItem(nomRonde,idRonde);
        }

        bdAgents.close();

        //Connexion à la base de données des utilisateurs à l'adresse 172.18.58.86

        bdAbonnes = QSqlDatabase::addDatabase("QMYSQL");
        bdAbonnes.setHostName(adresseIpAbonnes);
        bdAbonnes.setDatabaseName(nomBaseAbonnes);
        bdAbonnes.setUserName(userAbonnes);
        bdAbonnes.setPassword(mdpAbonnes);
        //Ouverture de la base par QT
        if(!bdAbonnes.open())
        {
            qDebug()<<"pb acces bd abonnes"<<bdAbonnes.lastError();
        }
        else
        {
            qDebug()<<"acces bd ok abonnes";
        }
        //requete pour récuperer le prénom et le nom des abonnés, leur email et leur id
        QSqlQuery requeteAbonnes("select abonnes_nom,abonnes_prenom,abonnes_id from Liste_abonnes;" );
        if (!requeteAbonnes.exec()){
            qDebug()<<"pb requete "<<requeteAbonnes.lastError();
        }
        //affichage des agents dans la comboBox

        while(requeteAbonnes.next())
        {
            nomAbonnes=requeteAbonnes.value("abonnes_nom").toString();
            prenomAbonnes=requeteAbonnes.value("abonnes_prenom").toString();
            //emailAbonnes=requeteAbonnes.value("abonnes_email").toString();
            idAbonnes=requeteAbonnes.value("abonnes_id").toInt();
            ui->comboBoxListeAbonnees->addItem(nomAbonnes + " " + prenomAbonnes,idAbonnes);
        }

        nbAbonnesSelectionne = 0;
    }
    else
    {
        qDebug()<<"fichier ini non valide";
    }
}
