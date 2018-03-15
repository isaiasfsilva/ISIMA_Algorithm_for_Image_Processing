<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html lang="#LANG">
<head>

        
	
  <title>#Projet d'Imagerie</title>
  <meta http-equiv="Content-Type" content="text/html; charset=#CHARSET">

	
  <link rel="stylesheet" href="./style.css" type="text/css">

	
  <meta name="KeyWords" content="ISIMA, TP, imagerie">

	
  <meta name="Description" content="compte rendu de projet de TP">
</head>

  <body>
<h1>
  
  
  Identification de visage à l'aide du contours active </h1>
<h3>FARIA SILVA Isaias, METZKER SOARES Paula, Promo 18 - F4</h3>
<h1 class="titrearticle"> Présentation du sujet </h1>

Dans le cadre du travail final du cours d'Algorithmes pour le Traitement d'Images, on s'intéresse au sujet de Contour Active, une technique de segmentation d'une région de l'image. En conséquence, inspiré de l'article [CHANG, Data], on souhaite mettre en place un programme d'identification de visages selon la coloration de la peau représentée par un modèle de coloration en échelle RGB. Pour être en accord aux pratiques effectuées au cours des classes, on utilise la bibliothèque CImg pour le traitement des images. On vise à mesurer la performance de du logiciel mis en place pour identifier le visage. Par ailleurs, on cible l'identification des limites de la méthode aussi bien que leur performance pour identifier d'autres objets dont la coloration soit la couleur de la peau. Des expérimentations sur des images réelles sont présentées ainsi bien que des images artificielles pour répondre aux problématiques et conjonctures établies.  <br>

<h1 class="titrearticle"> Méthode</h1>

 
<h2 class="soustitrearticle"> Partie théorique</h2>

La détection du visage est une tâche de plus en plus important pour les applications qui concerne le traitement d'image, en spécial pour le traitement de visage dont cette procédure est la prémiere étape d'une reconnaissance automatique du visage en général. Traditionnellement, le gradient d'une image fournit des contours très précis et est donc une des techniques les plus simples pour la détection d'objets d'une image à partir des contours obtenus. Cependant, le visage humain a une particularité face à des objets en général puisqu'il est décrit pour un modèle unique sous l'échelle RGB. On cherche donc à maximiser la performance et la précision de l'identification du visage face à cette caractéristique particulière. Alors, on se tient à l'identification des bornes de l'objet à partir des frontières entre la région faciale et les autres régions de l'image. Pour cela, on met en place le modèle derformable aussi connu comme modèle de contour active. 

Un contour actif est une courbe qui évolue à partir de sa forme initiale vers une forme la plus proche du contour de l'objet d'intérêt sous l'action de la force d'énergie sur cette courbe. Le modèle de contour active mis en place est appellé "Snakes" et il cherche à minimiser la fonction d'énergie dans la courbe modélisant l'objet d'intérêt. Ce modèle a été introduit par [KASS,1988]. On reprend les notations présentées par [CHANG, Data], puisque c'est l'article de référence pour l'implémentation proposée. On définit donc une image paramétrée par I(x,y) une valeur réelle à partir des coordonnées cartesiennes bidimensionnalles. La courbe de contour sera donc paramétrée comme C(p) = {x(p), y(p)}, où p est un paramètre arbitraire. À partir de l'equation d'énergie proposée par [KASS,1988], on peut écrire l'énergie du contour active comme: 

<a href="https://www.codecogs.com/eqnedit.php?latex=E&space;=&space;\alpha&space;\int_{0}^{1}&space;|C'(p)|^2&space;dp&space;&plus;&space;\beta&space;\int_{0}^{1}&space;|C''(p)|^2&space;dp-&space;\lambda&space;\int_{0}^{1}&space;|\bigtriangledown&space;I(x(p),y(p))&space;|^2&space;dp" target="_blank"><img src="https://latex.codecogs.com/gif.latex?E&space;=&space;\alpha&space;\int_{0}^{1}&space;|C'(p)|^2&space;dp&space;&plus;&space;\beta&space;\int_{0}^{1}&space;|C''(p)|^2&space;dp-&space;\lambda&space;\int_{0}^{1}&space;|\bigtriangledown&space;I(x(p),y(p))&space;|^2&space;dp" title="E = \alpha \int_{0}^{1} |C'(p)|^2 dp + \beta \int_{0}^{1} |C''(p)|^2 dp- \lambda \int_{0}^{1} |\bigtriangledown I(x(p),y(p)) |^2 dp" /></a>



<!-- Présentation de la méthode utilisée et référence aux travaux connexes (cf. biblio article)<br> -->

<br>

<h2 class="soustitrearticle"> Implémentation</h2>
<!--
Détails de l'implémentation (sous CImg)
-->

<h1 class="titrearticle"> Résultats</h1>

<h2 class="soustitrearticle"> Validation sur données de synthèse</h2>
<!--
Vous devez créer des données de synthèse pour évaluer votre méthode
-->
<br>

<h2 class="soustitrearticle"> Données réelles</h2>
<!--
Une fois la méthode évaluée, vous appliquerez votre méthode sur des images réelles
-->
<br>

<h1 class="titrearticle"> Discussion</h1>
<!--
Commentez vos résultats (d'un point de vue qualitatif <span style="font-weight: bold;">ET</span> quantitatif)
-->


Limites : <br>
 * La reconnaissance requiert la bonne initialization des paramètres aussi bien que le bon positionnement du contour initial.
<br>
* Convergence à des limites concaves, sinon divergence ou perturbation des résultats.
<br>


<h1 class="titrearticle"> Références</h1>

[CHANG, Data] Jae-Sik Chang1, Mu-Youl Lee1 , Chae-Hyun Moon1, Hye-Sun Park1, Kyung-Mi Lee2, Hang-Joon Kim. "Face detection using active contours"  <br>

[KASS,1988]M. Michael Kass, Andrew Witkin, Demetri Terzopoulos. "Snakes : Active contour models". International Journal of Computer Vision, 1 :321?332, 1988 <br>


</body>
</html>
