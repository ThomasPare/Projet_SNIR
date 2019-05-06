/**
  @file ihmrapportderonde.cpp
  @brief implémentation de la classe IHMRapportDeRonde
  @version 1.0
  @author LEVILLAIN Dimitri
  @date 25/03/2019
  @details Classe Principale
  */

#include "ihmrapportderonde.h"
#include "ui_ihmrapportderonde.h"

/**
 * @brief IHMRapportDeRonde::IHMRapportDeRonde
 * @param parent
 * @details constructeur
 */

IHMRapportDeRonde::IHMRapportDeRonde(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IHMRapportDeRonde)
{
    ui->setupUi(this);

    nombreAbonnesSelectionne=0;

    SetThemes();
    AjoutNomColonne();

    ui->comboBoxListeAbonnees->addItem("Choisir abonné");

    xmlConfig = new AccesXML();
    QFileInfo fichierVerif(CHEMINXML);

    if(fichierVerif.exists() && fichierVerif.isFile())
    {
        xmlConfig->LireConfiguration(CHEMINXML);
        configBDD = xmlConfig->ObtenirConfigurationBDD();
    }
    else
    {
        qDebug()<<"Erreur lors de l'ouverture du fichier de configuration.\nLe fichier \"configurations.xml\" n'existe pas.";
        exit(401);
    }

    mySqlBdd = new AccesBdd;

    if(mySqlBdd->ConnexionBdd(configBDD->ipServeurBDD, configBDD->portServeurBDD, configBDD->nomBDD, configBDD->utilisateurBDD, configBDD->motDePasseBDD))
    {
        qDebug()<<"Connexion Bdd Synchro OK !";
    }
    else
    {
        qDebug()<<"Erreur lors de l'ouverture de la base de données.\nRelancez l'application et si le problème persiste, contacter un administrateur.";
        exit(401);
    }
    RemplirListe();
    AfficherRondeDansTableau();


}
/**
 * @brief IHMRapportDeRonde::~IHMRapportDeRonde
 * @details destructeur
 */
IHMRapportDeRonde::~IHMRapportDeRonde()
{
    delete ui;
}
/**
 * @brief IHMRapportDeRonde::SetThemes
 * @details Ajouter style, cacher éléments ...
 */
void IHMRapportDeRonde::SetThemes()
{
    //setStyleSheet("QWidget { background: #404244; } ");

    //Cacher élément au lancement
    ui->calendarWidget->setVisible(false);
    ui->calendarWidgetFin->setVisible(false);
    ui->tableWidgetPointeaux->setVisible(false);
    ui->pushButtonFermerPhoto->setVisible(false);
    ui->pushButtonRetour->setVisible(false);
    ui->scrollAreaPhoto->setVisible(false);
    ui->pushButtonGenererPDF->setVisible(false);

    //Image bouton retour
    QPixmap iconeRetour(":/images/images/boutonRetour.png");
    QIcon buttonRetourIcone(iconeRetour);
    ui->pushButtonRetour->setIcon(buttonRetourIcone);
    ui->pushButtonRetour->setIconSize(iconeRetour.rect().size());
}
/**
 * @brief IHMRapportDeRonde::on_pushButtonDate_clicked
 * @details Affichage du calendrier lors de l'appuie sur le bouton
 */
void IHMRapportDeRonde::on_pushButtonDate_clicked()
{
    ui->calendarWidget->setVisible(true);
}
/**
 * @brief IHMRapportDeRonde::on_pushButtonDateFin_clicked
 * @details Affichage du calendrier lors de l'appuie sur le bouton
 */
void IHMRapportDeRonde::on_pushButtonDateFin_clicked()
{
    ui->calendarWidgetFin->setVisible(true);
}
/**
 * @brief IHMRapportDeRonde::on_calendarWidget_clicked
 * @param date
 * @details Affiche la date sur le bouton
 */
void IHMRapportDeRonde::on_calendarWidget_clicked(const QDate &date)
{
    ui->pushButtonDate->setText(date.toString());
    ui->calendarWidget->setVisible(false);
}
/**
 * @brief IHMRapportDeRonde::on_calendarWidgetFin_clicked
 * @param date
 * @details Affiche la date sur le bouton
 */
void IHMRapportDeRonde::on_calendarWidgetFin_clicked(const QDate &date)
{
    ui->pushButtonDateFin->setText(date.toString());
    ui->calendarWidgetFin->setVisible(false);
}
/**
 * @brief IHMRapportDeRonde::AjoutNomColonne
 * @details Ajout des noms aux colonnes
 */
void IHMRapportDeRonde::AjoutNomColonne()
{
    ui->tableWidgetRonde->setColumnCount(3);
    ui->tableWidgetRonde->setHorizontalHeaderLabels(nomColonneRonde);

    ui->tableWidgetPointeaux->setColumnCount(5);
    ui->tableWidgetPointeaux->setHorizontalHeaderLabels(nomColonnePointeaux);

    ui->tableWidgetRonde->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidgetPointeaux->horizontalHeader()->setStretchLastSection(true);

    for(int i=0;i<4;i++)
    {
        ui->tableWidgetRonde->setColumnWidth(i,180);
        ui->tableWidgetPointeaux->setColumnWidth(i,120);
    }
    ui->tableWidgetPointeaux->setColumnWidth(2,150);
    ui->tableWidgetPointeaux->setColumnWidth(4,25);

    //Parametre tableau
    ui->tableWidgetRonde->setSelectionBehavior(QAbstractItemView::SelectRows);                   //Sélection par ligne
    ui->tableWidgetRonde->setSelectionMode(QAbstractItemView::SingleSelection);                  //Sélection d'une seul ligne à la fois

    ui->tableWidgetPointeaux->setSelectionBehavior(QAbstractItemView::SelectRows);                   //Sélection par ligne
    ui->tableWidgetPointeaux->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->tableWidgetRonde->setEditTriggers(false);                                                //Edition du contenu impossible
    ui->tableWidgetPointeaux->setEditTriggers(false);                                                //Edition du contenu impossible
}
/**
 * @brief IHMRapportDeRonde::on_lineEditAgent_textChanged
 * @param arg1
 * @details Supprime les éléments des différentes listes si ils ne correspondent pas au debut du nom de l'agent.
 */
void IHMRapportDeRonde::on_lineEditAgent_textChanged(const QString &arg1)
{
    int i = 0;
    QString debutAgent;

    RemplirListe();

    while(i<listeNomAgents.size())
    {
        //liste du début de chaques Agents
        debutAgent = listeNomAgents.at(i).mid(0,arg1.size());
        if(debutAgent != arg1)
        {
            //ui->tableWidgetRonde->removeRow(i);
            listeNomAgents.removeAt(i);
            listeNomRondes.removeAt(i);
            listeDate.removeAt(i);
            listeIdRonde.removeAt(i);
            listeIdAgent.removeAt(i);
            //qDebug()<<"Agent : "<<listeNomAgents.at(i);
            qDebug()<<i;
        }
        i++;
    }
    //qDebug()<<"Après : "<<listeNomAgents<<listeNomRondes;
    AfficherRondeDansTableau();
}
/**
 * @brief IHMRapportDeRonde::on_lineEditRonde_textChanged
 * @param arg1
 * @details Supprime les éléments des différentes liste si ils ne correspondent pas au débur du nom de la ronde
 */
void IHMRapportDeRonde::on_lineEditRonde_textChanged(const QString &arg1)
{
    int i = 0;
    QString debutRonde;

    RemplirListe();

    while(i<listeNomRondes.size())
    {
        //liste du debut de chaques rondes
        debutRonde = listeNomRondes.at(i).mid(0,arg1.size());
        if(debutRonde != arg1)
        {
            //ui->tableWidgetRonde->removeRow(i);
            listeNomAgents.removeAt(i);
            listeNomRondes.removeAt(i);
            listeDate.removeAt(i);
            listeIdRonde.removeAt(i);
            listeIdAgent.removeAt(i);
        }
        i++;
    }
    //qDebug()<<"Après : "<<listeNomAgents<<listeNomRondes;
    AfficherRondeDansTableau();
}
/**
 * @brief IHMRapportDeRonde::RemplirListe
 * @details Efface les différentes listes et recupere dans les listes les noms d'agents, rondes et dates.
 */
void IHMRapportDeRonde::RemplirListe()
{
    //vider listes
    listeNomAgents.clear();
    listeNomRondes.clear();
    listeDate.clear();
    listeIdRonde.clear();
    listeIdAgent.clear();

    mySqlBdd->ObtenirNomAgentsNomRondesDate();

    listeNomAgents = mySqlBdd->ObtenirListeNomAgent();
    listeNomRondes = mySqlBdd->ObtenirListeNomRonde();
    listeDate = mySqlBdd->ObtenirListeDate();
    listeIdRonde = mySqlBdd->ObtenirListeIdRonde();
    listeIdAgent = mySqlBdd->ObtenirListeIdAgent();

    /*qDebug()<<"liste agent : "<<listeNomAgents;
    qDebug()<<"liste ronde : "<<listeNomRondes;
    qDebug()<<"liste date : "<<listeDate;
    qDebug()<<"liste id ronde : "<<listeIdRonde;
    qDebug()<<"liste id agent : "<<listeIdAgent;*/
}
/**
 * @brief IHMRapportDeRonde::AfficherRondeDansTableau
 * @details Affichage des différentes rondes dans le tableau grâce aux listes
 */
void IHMRapportDeRonde::AfficherRondeDansTableau()
{
    int i = 0;

    //Cacher bouton
    ui->pushButtonRetour->setVisible(false);
    //Afficher tableau ronde et cacher tableau pointeaux
    ui->tableWidgetRonde->setVisible(true);
    ui->tableWidgetPointeaux->setVisible(false);


    //Enlever contenu tableau Ronde
    while(ligneRonde>=0)
    {
        ui->tableWidgetRonde->removeRow(ligneRonde);
        ligneRonde--;
    }
    ligneRonde = 0;

    //Affichage du tableau
    while(i<listeNomAgents.size())
    {
        ui->tableWidgetRonde->setColumnCount(3);
        // ajouter une ligne au tableau
        ui->tableWidgetRonde->insertRow(ui->tableWidgetRonde->rowCount());
        int col=0;
        // ajouter nom ronde
        QTableWidgetItem *nomRondeItem = new QTableWidgetItem(listeNomRondes.at(i));
        nomRondeItem->setTextAlignment(Qt::AlignHCenter);
        ui->tableWidgetRonde->setItem(ligneRonde,col,nomRondeItem);
        col++;
        // ajouter nom agent
        QTableWidgetItem *nomAgentItem = new QTableWidgetItem(listeNomAgents.at(i));
        nomAgentItem->setTextAlignment(Qt::AlignHCenter);
        ui->tableWidgetRonde->setItem(ligneRonde,col,nomAgentItem);
        col++;
        // ajouter date
        QTableWidgetItem *dateItem = new QTableWidgetItem(listeDate.at(i).toString());
        dateItem->setTextAlignment(Qt::AlignHCenter);
        ui->tableWidgetRonde->setItem(ligneRonde,col,dateItem);
        col++;
        ligneRonde++;
        i++;
    }
}

/**
 * @brief IHMRapportDeRonde::on_tableWidgetRonde_cellClicked
 * @param row
 * @param column
 * @details Affiche les poiteaux de la ronde
 */
void IHMRapportDeRonde::on_tableWidgetRonde_cellClicked(int row)
{
    ui->tableWidgetRonde->setVisible(false);
    ui->pushButtonRetour->setVisible(true);
    ui->tableWidgetPointeaux->setVisible(true);
    ui->pushButtonGenererPDF->setVisible(true);

    mySqlBdd->ObtenirIdPointeauDate(listeIdAgent.at(row),listeIdRonde.at(row));
    listeIdPointeaux.clear();
    listeDatePointeau.clear();
    listeIdPointeaux = mySqlBdd->ObtenirListeIdPointeaux();
    listeDatePointeau = mySqlBdd->ObtenirListeDatePointeau();

    qDebug()<<"liste id pointeaux : "<<listeIdPointeaux;
    qDebug()<<"liste date pointeau : "<<listeDatePointeau;

    AfficherPointeauxDansTableau();
}
/**
 * @brief IHMRapportDeRonde::AfficherPointeauxDansTableau
 * @details Afficher pointeaux dans tableau, bouton retour et cacher tableau ronde.
 */

void IHMRapportDeRonde::AfficherPointeauxDansTableau()
{
    int i = 0;

    //Afficher bouton
    ui->pushButtonRetour->setVisible(true);
    //Cacher tableau ronde et afficher tableau pointeaux
    ui->tableWidgetRonde->setVisible(false);
    ui->tableWidgetPointeaux->setVisible(true);

    //Enlever contenu tableau Pointeaux
    while(lignePointeaux>=0)
    {
        ui->tableWidgetPointeaux->removeRow(lignePointeaux);
        lignePointeaux--;
    }
    lignePointeaux = 0;

    while(i<listeIdPointeaux.size())
    {
        qDebug()<<listeIdPointeaux.at(i);
        ui->tableWidgetPointeaux->insertRow(i);
        int col=0;
        // ajouter Numéro Pointeaux
        QTableWidgetItem *numeroPointeau = new QTableWidgetItem(QString::number(listeIdPointeaux.at(i)));
        ui->tableWidgetPointeaux->setItem(lignePointeaux,col,numeroPointeau);
        numeroPointeau->setTextAlignment(Qt::AlignHCenter);
        col++;
        //ajouter retard
        QTableWidgetItem *retard = new QTableWidgetItem(" ");
        ui->tableWidgetPointeaux->setItem(lignePointeaux,col,retard);
        retard->setTextAlignment(Qt::AlignHCenter);
        col++;
        //ajouter heure de pointage
        QTableWidgetItem *heurePointageItem = new QTableWidgetItem(listeDatePointeau.at(i).toString());
        ui->tableWidgetPointeaux->setItem(lignePointeaux,col,heurePointageItem);
        heurePointageItem->setTextAlignment(Qt::AlignHCenter);
        col++;
        //ajouter commentaire
        QTableWidgetItem *commentaire = new QTableWidgetItem(" ");
        ui->tableWidgetPointeaux->setItem(lignePointeaux,col,commentaire);
        commentaire->setTextAlignment(Qt::AlignHCenter);
        col++;
        //ajouter photo1
        QTableWidgetItem *photo = new QTableWidgetItem;
        photo->setData(Qt::DecorationRole, QPixmap(":/images/images/logoAppareil.png"));
        ui->tableWidgetPointeaux->setItem(1,col, photo);
        //ajouter photo2
        QTableWidgetItem *photoD = new QTableWidgetItem;
        photoD->setData(Qt::DecorationRole, QPixmap(":/images/images/logoAppareil.png"));
        ui->tableWidgetPointeaux->setItem(2,col, photoD);
        lignePointeaux++;
        i++;
    }
}
/**
 * @brief IHMRapportDeRonde::on_tableWidgetPointeaux_cellClicked
 * @param row
 * @param column
 * @details Affichage photo selon la ronde cliqué
 */
void IHMRapportDeRonde::on_tableWidgetPointeaux_cellClicked(int row)
{
    int i = 0;
    QList<QCheckBox*> listeCheckBoxImage;

    numeroImage = row;

    QList<QIcon> listeImage;
    QString chemin;

    for(int j=0;j<9;j++)
    {
        chemin = "/home/USERS/ELEVES/SNIR2017/dlevillain/SNIR2/projet/copieV3.6/ihm_supervisionV3/photo/pointeau"+QString::number(listeIdPointeaux.at(numeroImage))+"/"+QString::number(j)+".jpg";
        QIcon image(chemin);
        listeImage.append(image);
    }

    ui->pushButtonFermerPhoto->setStyleSheet("background-color: red; color: white");
    ui->pushButtonFermerPhoto->setVisible(true);
    ui->labelPhoto->setPixmap("/home/USERS/ELEVES/SNIR2017/dlevillain/SNIR2/projet/copieV3.6/ihm_supervisionV3/photo/pointeau"+QString::number(listeIdPointeaux.at(numeroImage))+"/0.jpg");
    ui->scrollAreaPhoto->setVisible(true);

    QHBoxLayout *layout = new QHBoxLayout;
    QWidget *widgetPhoto = new QWidget();

    ui->scrollAreaPhoto->setWidget(widgetPhoto);
    widgetPhoto->setLayout(layout);

    foreach(QIcon img, listeImage)
    {
        //création boutons
        QPushButton *boutonImage = new QPushButton();
        //création checkbbox
        QCheckBox *checkBoxImage = new QCheckBox();

        boutonImage->setFixedSize(QSize(65,45));
        boutonImage->setFlat(true);
        //insertion de l'image dans le bouton
        boutonImage->setIcon(img);
        boutonImage->setIconSize(QSize(65,45));

        listeBoutonImage.append(boutonImage);
        listeCheckBoxImage.append(checkBoxImage);

        layout->addWidget(listeCheckBoxImage[i], Qt::AlignLeft);
        layout->addWidget(listeBoutonImage[i], Qt::AlignLeft);

        connect(listeBoutonImage[i],&QPushButton::clicked,this,&IHMRapportDeRonde::OnQPushButtonPhoto_clicked);

        i++;
    }
}

/**
 * @brief SupervisionRondier::OnQPushButtonPhoto_clicked
 * @details Permet d'afficher la photo cliqué dans la liste déroulante.
 */
void IHMRapportDeRonde::OnQPushButtonPhoto_clicked()
{
    QPushButton *b=qobject_cast<QPushButton*>(sender());
    QString chemin = "/home/USERS/ELEVES/SNIR2017/dlevillain/SNIR2/projet/proj/rapportderonde/pointeau"+QString::number(listeIdPointeaux.at(numeroImage))+"/"+QString::number(listeBoutonImage.indexOf(b))+".jpg";
    ui->labelPhoto->setPixmap(chemin);
    ui->labelPhoto->setScaledContents(true);
    ui->labelPhoto->setVisible(true);
    ui->pushButtonFermerPhoto->setVisible(true);
}
/**
 * @brief IHMRapportDeRonde::on_pushButtonFermerPhoto_clicked
 * @details Fermer photo
 */
void IHMRapportDeRonde::on_pushButtonFermerPhoto_clicked()
{
    ui->labelPhoto->setVisible(false);
    ui->pushButtonFermerPhoto->setVisible(false);
}

/**
 * @brief SupervisionRondier::on_pushButtonApercuPDF_clicked
 * @details Lance le lecteur pdf avec le chemin du fichier en argument
 */
void IHMRapportDeRonde::on_pushButtonApercuPDF_clicked()
{
    QString lecteurPDF = "/usr/bin/xreader";
    QStringList arguments;
    arguments << "/home/USERS/ELEVES/SNIR2017/dlevillain/Documents/pdf/mydoc.pdf";
    QProcess *execLecteurPDF = new QProcess(this);
    execLecteurPDF->start(lecteurPDF, arguments);
}

/**
 * @brief IHMRapportDeRonde::on_pushButtonRetour_clicked
 * @details Permet de revenir sur le tableau de ronde
 */
void IHMRapportDeRonde::on_pushButtonRetour_clicked()
{
    //Cacher bouton
    ui->pushButtonRetour->setVisible(false);
    //Afficher tableau ronde et cacher tableau pointeaux
    ui->tableWidgetRonde->setVisible(true);
    ui->tableWidgetPointeaux->setVisible(false);
}

void IHMRapportDeRonde::on_pushButtonValiderListe_clicked()
{
    QString ronde = ui->comboBoxRondeASelectionner->currentText();
    QString email;

    for(int i = 0; i < nombreAbonnesSelectionne; i++)
    {
        email = ui->listWidget->item(i)->text();
        BddAbonnes->SauvegardeMail(ronde, email);
    }
    ui->listWidget->clear();
    RemplirListeAbonnes();
}

void IHMRapportDeRonde::on_pushButtonSupprimerAbonne_clicked()
{    
    if((ui->listWidget->count()>0))
    {
        ui->comboBoxListeAbonnees->addItem(BddAbonnes->QListToComboBoxAbonnes(ui->listWidget->currentItem()->text()));
        delete ui->listWidget->currentItem();
        nombreAbonnesSelectionne--;
    }
    qDebug()<<nombreAbonnesSelectionne;
}

void IHMRapportDeRonde::on_pushButtonRafraichirInterface_clicked()
{

}

void IHMRapportDeRonde::RemplirListeRonde()
{
    ui->comboBoxRondeASelectionner->clear();

    mySqlBdd->ObtenirNomRondes();
    QList<QString> ListeNomRondes = mySqlBdd->ObtenirListeNomRonde();
    QList<int> ListeIdRondes = mySqlBdd->ObtenirListeIdRonde();

    for(int i =0; i < ListeNomRondes.count(); i++)
    {
        QString ListeNomRondesRecup = ListeNomRondes[i];
        int ListeIdRondesRecup = ListeIdRondes[i];

        ui->comboBoxRondeASelectionner->addItem(ListeNomRondesRecup, ListeIdRondesRecup);
        qDebug()<<ListeIdRondesRecup;
    }
}

void IHMRapportDeRonde::RemplirListeAbonnes()
{
    ui->comboBoxListeAbonnees->clear();
    ui->comboBoxListeAbonnees->addItem("Choisir abonné");

    BddAbonnes->ObtenirNomAbonnes();
    QList<QString> ListeNomAbonne = BddAbonnes->ObtenirListeNomAbonnes();
    QList<int> ListeIdAbonne = BddAbonnes->ObtenirListeIdAbonnes();

    for(int i=0; i < ListeNomAbonne.count(); i++)
    {
        QString ListeNomAbonneRecup = ListeNomAbonne[i];
        int ListeIdAbonneRecup = ListeIdAbonne[i];

        ui->comboBoxListeAbonnees->addItem(ListeNomAbonneRecup, ListeIdAbonneRecup);
    }


}

void IHMRapportDeRonde::on_tabWidget_currentChanged(int index)
{
    //Partie XML
    xmlConfig = new AccesXML();
    QFileInfo fichierVerif(CHEMINXML);

    if(fichierVerif.exists() && fichierVerif.isFile())
    {
        xmlConfig->LireConfiguration(CHEMINXML);
        configBDD = xmlConfig->ObtenirConfigurationBDD();
    }
    else
    {
        qDebug()<<"Erreur lors de l'ouverture du fichier de configuration.\nLe fichier \"configurations.xml\" n'existe pas.";
        exit(401);
    }

    if (index != 7)
    {
        //Partie sql
        mySqlBdd = new AccesBdd;

        if(mySqlBdd->ConnexionBdd(configBDD->ipServeurBDD, configBDD->portServeurBDD, configBDD->nomBDD, configBDD->utilisateurBDD, configBDD->motDePasseBDD))
        {
            qDebug()<<"Connexion Bdd Synchro OK !";
        }
        else
        {
            qDebug()<<"Erreur lors de l'ouverture de la base de données.\nRelancez l'application et si le problème persiste, contacter un administrateur.";
            exit(401);
        }
        RemplirListe();
        AfficherRondeDansTableau();
    }
    else
    {
        //Partie sql
        RemplirListeRonde();


        BddAbonnes = new AccesBdd;

        if(BddAbonnes->ConnexionBdd(configBDD->ipServeurBDDAbonnes, configBDD->portServeurBDDAbonnes, configBDD->nomBDDAbonnes, configBDD->utilisateurBDDAbonnes, configBDD->motDePasseBDDAbonnes))
        {
            qDebug()<<"Connexion Bdd abonnes OK !";
        }
        else
        {
            qDebug()<<"Erreur lors de l'ouverture de la base de données.\nRelancez l'application et si le problème persiste, contacter un administrateur.";
            exit(401);
        }
        RemplirListeAbonnes();
    }


}

void IHMRapportDeRonde::on_comboBoxListeAbonnees_activated(const QString &arg1)
{
    if(!(arg1 == "Choisir abonné"))
    {
        QSqlQuery requeteEmail;

        if(!(arg1 == "Choisir abonné"))
        {
            QList <QString> listeNomPrenom=arg1.split(" ");
            qDebug()<<"nom : "<<listeNomPrenom[0]<<" prenom : "<<listeNomPrenom[1];
            QString nom=listeNomPrenom[0];
            QString prenom=listeNomPrenom[1];
            ui->listWidget->addItem(BddAbonnes->comboBoxAbonnesToQlist(nom,prenom));
            ui->comboBoxListeAbonnees->removeItem(ui->comboBoxListeAbonnees->currentIndex());
            nombreAbonnesSelectionne++;
            qDebug()<<nombreAbonnesSelectionne;
        }
    }
}
