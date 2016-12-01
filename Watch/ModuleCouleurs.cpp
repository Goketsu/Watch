
/* Module de gestion des couleurs         */

#include <stdlib.h>

static float blanc[] = { 1.0F,1.0F,1.0F,1.0F };
static float jaune[] = { 1.0F,1.0F,0.0F,1.0F };
static float cyan[] = { 0.0F,1.0F,1.0F,1.0F };
static float magenta[] = { 1.0F,0.0F,1.0F,1.0F };
static float rouge[] = { 1.0F,0.0F,0.0F,1.0F };
static float vert[] = { 0.0F,1.0F,0.0F,1.0F };
static float bleu[] = { 0.0F,0.0F,1.0F,1.0F };
static float noir[] = { 0.0F,0.0F,0.0F,1.0F };
static float grisTresFonce[] = { 0.1F,0.1F,0.1F,1.0F };
static float grisFonce[] = { 0.25F,0.25F,0.25F,1.0F };
static float grisMoyen[] = { 0.5F,0.5F,0.5F,1.0F };
static float grisClair[] = { 0.75F,0.75F,0.75F,1.0F };
static float jauneClair[] = { 0.75F,0.75F,0.0F,1.0F };
static float jauneFonce[] = { 0.5F,0.5F,0.0F,1.0F };
static float cyanFonce[] = { 0.0F,0.5F,0.5F,1.0F };
static float magentaFonce[] = { 0.5F,0.0F,0.5F,1.0F };
static float rougeFonce[] = { 0.5F,0.0F,0.0F,1.0F };
static float vertFonce[] = { 0.0F,0.5F,0.0F,1.0F };
static float bleuFonce[] = { 0.0F,0.0F,0.5F,1.0F };
static float brun[] = { 0.8F,0.6F,0.4F,1.0F };
static float rose[] = { 1.0F,0.65F,0.65F,1.0F };
static float roseFonce[] = { 0.6F,0.25F,0.25F,1.0F };
static float bleuCiel[] = { 0.5F,0.5F,1.0F,1.0F };
static float bleuCielFonce[] = { 0.25F,0.25F,0.5F,1.0F };

float *couleurBlanc(void) {
  return(blanc);
}

float *couleurJaune(void) {
  return(jaune);
}

float *couleurCyan(void) {
  return(cyan);
}

float *couleurMagenta(void) {
  return(magenta);
}

float *couleurRouge(void) {
  return(rouge);
}

float *couleurVert(void) {
  return(vert);
}

float *couleurBleu(void) {
  return(bleu);
}

float *couleurNoir(void) {
  return(noir);
}

float *couleurGrisTresFonce(void) {
  return(grisTresFonce);
}

float *couleurGrisFonce(void) {
  return(grisFonce);
}

float *couleurGrisMoyen(void) {
  return(grisMoyen);
}

float *couleurGrisClair(void) {
  return(grisClair);
}

float *couleurJauneClair(void) {
  return(jauneClair);
}

float *couleurJauneFonce(void) {
  return(jauneFonce);
}

float *couleurCyanFonce(void) {
  return(cyanFonce);
}

float *couleurMagentaFonce(void) {
  return(magentaFonce);
}

float *couleurRougeFonce(void) {
  return(rougeFonce);
}

float *couleurVertFonce(void) {
  return(vertFonce);
}

float *couleurBleuFonce(void) {
  return(bleuFonce);
}

float *couleurBrun(void) {
  return(brun);
}

float *couleurRose(void) {
  return(rose);
}

float *couleurRoseFonce(void) {
  return(roseFonce);
}

float *couleurBleuCiel(void) {
  return(bleuCiel);
}

float *couleurBleuCielFonce(void) {
  return(bleuCielFonce);
}

float *couleurBleuCiel(float alpha) {
  static float coul[4];
  coul[0] = bleuCiel[0];
  coul[1] = bleuCiel[1];
  coul[2] = bleuCiel[2];
  coul[3] = alpha;
  return(coul);
}

float *couleurBleuCielFonce(float alpha) {
  static float coul[4];
  coul[0] = bleuCielFonce[0];
  coul[1] = bleuCielFonce[1];
  coul[2] = bleuCielFonce[2];
  coul[3] = alpha;
  return(coul);
}

float *couleurRose(float alpha) {
  static float coul[4];
  coul[0] = rose[0];
  coul[1] = rose[1];
  coul[2] = rose[2];
  coul[3] = alpha;
  return(coul);
}

float *couleurRoseFonce(float alpha) {
  static float coul[4];
  coul[0] = roseFonce[0];
  coul[1] = roseFonce[1];
  coul[2] = roseFonce[2];
  coul[3] = alpha;
  return(coul);
}

float *couleurGris(float niveau,float alpha) {
  static float coul[4];
  coul[0] = niveau;
  coul[1] = niveau;
  coul[2] = niveau;
  coul[3] = alpha;
  return(coul);
}

float *couleurGrisFonce(float alpha) {
  static float coul[4];
  coul[0] = grisFonce[0];
  coul[1] = grisFonce[1];
  coul[2] = grisFonce[2];
  coul[3] = alpha;
  return(coul);
}

float *couleurGrisTresFonce(float alpha) {
  static float coul[4];
  coul[0] = grisTresFonce[0];
  coul[1] = grisTresFonce[1];
  coul[2] = grisTresFonce[2];
  coul[3] = alpha;
  return(coul);
}

float *couleurGrisMoyen(float alpha) {
  static float coul[4];
  coul[0] = grisMoyen[0];
  coul[1] = grisMoyen[1];
  coul[2] = grisMoyen[2];
  coul[3] = alpha;
  return(coul);
}

float *couleurGrisClair(float alpha) {
  static float coul[4];
  coul[0] = grisClair[0];
  coul[1] = grisClair[1];
  coul[2] = grisClair[2];
  coul[3] = alpha;
  return(coul);
}

float *couleurBlanc(float alpha) {
  static float coul[4];
  coul[0] = blanc[0];
  coul[1] = blanc[1];
  coul[2] = blanc[2];
  coul[3] = alpha;
  return(coul);
}

float *couleurJaune(float alpha) {
  static float coul[4];
  coul[0] = jaune[0];
  coul[1] = jaune[1];
  coul[2] = jaune[2];
  coul[3] = alpha;
  return(coul);
}

float *couleurCyan(float alpha) {
  static float coul[4];
  coul[0] = cyan[0];
  coul[1] = cyan[1];
  coul[2] = cyan[2];
  coul[3] = alpha;
  return(coul);
}

float *couleurMagenta(float alpha) {
  static float coul[4];
  coul[0] = magenta[0];
  coul[1] = magenta[1];
  coul[2] = magenta[2];
  coul[3] = alpha;
  return(coul);
}

float *couleurNoir(float alpha) {
  static float coul[4];
  coul[0] = noir[0];
  coul[1] = noir[1];
  coul[2] = noir[2];
  coul[3] = alpha;
  return(coul);
}

float *couleurVert(float alpha) {
  static float coul[4];
  coul[0] = vert[0];
  coul[1] = vert[1];
  coul[2] = vert[2];
  coul[3] = alpha;
  return(coul);
}

float *couleurRouge(float alpha) {
  static float coul[4];
  coul[0] = rouge[0];
  coul[1] = rouge[1];
  coul[2] = rouge[2];
  coul[3] = alpha;
  return(coul);
}

float *couleurBleu(float alpha) {
  static float coul[4];
  coul[0] = bleu[0];
  coul[1] = bleu[1];
  coul[2] = bleu[2];
  coul[3] = alpha;
  return(coul);
}

float *couleurJauneClair(float alpha) {
  static float coul[4];
  coul[0] = jauneClair[0];
  coul[1] = jauneClair[1];
  coul[2] = jauneClair[2];
  coul[3] = alpha;
  return(coul);
}

float *couleurJauneFonce(float alpha) {
  static float coul[4];
  coul[0] = jauneFonce[0];
  coul[1] = jauneFonce[1];
  coul[2] = jauneFonce[2];
  coul[3] = alpha;
  return(coul);
}

float *couleurCyanFonce(float alpha) {
  static float coul[4];
  coul[0] = cyanFonce[0];
  coul[1] = cyanFonce[1];
  coul[2] = cyanFonce[2];
  coul[3] = alpha;
  return(coul);
}

float *couleurMagentaFonce(float alpha) {
  static float coul[4];
  coul[0] = magentaFonce[0];
  coul[1] = magentaFonce[1];
  coul[2] = magentaFonce[2];
  coul[3] = alpha;
  return(coul);
}

float *couleurVertFonce(float alpha) {
  static float coul[4];
  coul[0] = vertFonce[0];
  coul[1] = vertFonce[1];
  coul[2] = vertFonce[2];
  coul[3] = alpha;
  return(coul);
}

float *couleurRougeFonce(float alpha) {
  static float coul[4];
  coul[0] = rougeFonce[0];
  coul[1] = rougeFonce[1];
  coul[2] = rougeFonce[2];
  coul[3] = alpha;
  return(coul);
}

float *couleurBleuFonce(float alpha) {
  static float coul[4];
  coul[0] = bleuFonce[0];
  coul[1] = bleuFonce[1];
  coul[2] = bleuFonce[2];
  coul[3] = alpha;
  return(coul);
}


float *newCouleur(float r,float v,float b,float a) {
  static float coul[4];
  coul[0] = r;
  coul[1] = v;
  coul[2] = b;
  coul[3] = a;
  return(coul);
}

float *newCouleur(float *c) {
  float *coul =(float *) calloc(4,sizeof(float));
  coul[0] = c[0];
  coul[1] = c[1];
  coul[2] = c[2];
  coul[3] = c[3];
  return(coul);
}

void freeCouleur(float *c) {
  free(c);
}

float **paletteColoree(void) {
  int i;
  float **p =(float **) calloc(256,sizeof(float *));
  for ( i = 0 ; i <= 51 ; i++ ) {
    p[i] =(float *) calloc(4,sizeof(float));
    p[i][0] = 0.0F;
    p[i][1] = 1.0F;
    p[i][2] = i/51.0F;
    p[i][3] = 1.0F; }
  for ( ; i <= 102 ; i++ ) {
    p[i] =(float *) calloc(4,sizeof(float));
    p[i][0] = 0.0F;
    p[i][1] = 1.0F-(i-51)/51.0F;
    p[i][2] = 1.0F;
    p[i][3] = 1.0F; }
  for ( ; i <= 153 ; i++ ) {
    p[i] =(float *) calloc(4,sizeof(float));
    p[i][0] = (i-102)/51.0F;
    p[i][1] = 0.0F;
    p[i][2] = 1.0F;
    p[i][3] = 1.0F; }
  for ( ; i <= 204 ; i++ ) {
    p[i] =(float *) calloc(4,sizeof(float));
    p[i][0] = 1.0F;
    p[i][1] = 0.0F;
    p[i][2] = 1.0F-(i-153)/51.0F;
    p[i][3] = 1.0F; }
  for ( ; i <= 255 ; i++ ) {
    p[i] =(float *) calloc(4,sizeof(float));
    p[i][0] = 1.0F;
    p[i][1] = (i-204)/51.0F;
    p[i][2] = 0.0F;
    p[i][3] = 1.0F; }
  return(p);
}

float **paletteGrisee(void) {
  int i;
  float **p =(float **) calloc(256,sizeof(float *));
  for ( i = 0 ; i <= 255 ; i++ ) {
    p[i] =(float *) calloc(4,sizeof(float));
    p[i][0] = p[i][1] = p[i][2] = i/255.0F;
    p[i][3] = 1.0F; }
  return(p);
}
