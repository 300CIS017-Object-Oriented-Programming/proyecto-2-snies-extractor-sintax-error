# Informe de Refactorización y Análisis de Código
**Mateo Monroy, Juan Pablo Naranjo y César Zuluaga**

Con base en el análisis de **SonarCloud**, la versión en la nube de SonarQube, identificamos como principales objetivos de refactorización:

- Los métodos para leer y crear archivos del **Gestor Csv**.
- La forma de organizar los **programas académicos y consolidados**.
- La gran **duplicación de código** a lo largo de todo el código fuente.

## Refactorización Aplicada

Para resolver estos problemas, tomamos las siguientes decisiones:

1. **Rehacer las clases `Consolidado` y `Programa`**, asociándolos mediante un **mapa**.
2. **Unificar la lectura de archivos** en el **Gestor de Archivos** mediante una única función.
3. **Eliminar código repetido** creando métodos privados dentro de cada clase para **reciclar código**.

## Resultados

Estas prácticas lograron reducir de manera significativa la **duplicación de código**. Sin embargo:

- **La complejidad ciclomática** aumentó ligeramente debido a los nuevos requerimientos y al diseño más robusto.
- La **organización, documentación y eficiencia** del código mejoraron notablemente gracias a los conocimientos adquiridos en clase y la implementación práctica de los mismos.

---
