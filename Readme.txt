Made by Cédric Chopin / Guillaume Trem / Etienne Gibiat

Le but de ce projet est de réaliser un programme de composition d’images, c'est-à- dire,
combiner plusieurs images prises d’un même point de vue, en une seule, qui reconstitue
l’action du sujet.

Les options à utiliser sont les suivantes : 

-dir + nom du dossier sous la forme "..\\Photos\\dossier" ou "C:\\..." (Ne pas oublier les "\\")

-files + nom des fichiers

-fading opaque / plus / moins ( Plus : permet le fading de plus en plus transparent, moins : de plus en plus opaque, opaque : tout les sujets sont opaques )

-overlap + nombre : nombre maximals de superpositions

-distance : les sujets ne se touchent pas

Mettre le nom de l'image résultat à la fin de la ligne de commande

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Corona est une librairie d'ouverture, de gestion de pixels et de sauvegarde d'images pour C++.
Le choix de cette librairie est due au manque de documentation que l'on a trouvé sur internet sur les autres librairie et du fait que l'on avait pas le droit d'utiliser OpenCV (sans oublier le nom iconique de cette librairie).

Voici des liens concernant sa documentation :
- http://corona.sourceforge.net/corona-1.0.2-doxygen/main.html
- http://corona.sourceforge.net/documentation.php