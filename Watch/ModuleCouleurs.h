
/* Module de gestion des couleurs         */

#ifndef MODULECOULEURS
#define MODULECOULEURS

float *couleurBlanc(void);
float *couleurCyan(void);
float *couleurMagenta(void);
float *couleurJaune(void);
float *couleurRouge(void);
float *couleurVert(void);
float *couleurBleu(void);
float *couleurNoir(void);
float *couleurGrisTresFonce(void);
float *couleurGrisFonce(void);
float *couleurGrisMoyen(void);
float *couleurGrisClair(void);
float *couleurJauneClair(void);
float *couleurJauneFonce(void);
float *couleurCyanFonce(void);
float *couleurMagentaFonce(void);
float *couleurRougeFonce(void);
float *couleurVertFonce(void);
float *couleurBleuFonce(void);
float *couleurBrun(void);
float *couleurRose(void);
float *couleurRoseFonce(void);
float *couleurBleuCiel(void);
float *couleurBleuCielFonce(void);

float *couleurBlanc(float alpha);
float *couleurCyan(float alpha);
float *couleurMagenta(float alpha);
float *couleurJaune(float alpha);
float *couleurRouge(float alpha);
float *couleurVert(float alpha);
float *couleurBleu(float alpha);
float *couleurNoir(float alpha);
float *couleurGrisTresFonce(float alpha);
float *couleurGrisFonce(float alpha);
float *couleurGrisMoyen(float alpha);
float *couleurGrisClair(float alpha);
float *couleurJauneClair(float alpha);
float *couleurJauneFonce(float alpha);
float *couleurCyanFonce(float alpha);
float *couleurMagentaFonce(float alpha);
float *couleurRougeFonce(float alpha);
float *couleurVertFonce(float alpha);
float *couleurBleuFonce(float alpha);
float *couleurBrun(float alpha);
float *couleurRose(float alpha);
float *couleurRoseFonce(float alpha);
float *couleurBleuCiel(float alpha);
float *couleurBleuCielFonce(float alpha);
float *couleurGris(float niveau,float alpha);
float *newCouleur(float *c);
void freeCouleur(float *c);
float *newCouleur(float r,float v,float b,float a);
float **paletteColoree(void);
float **paletteGrisee(void);

#endif
