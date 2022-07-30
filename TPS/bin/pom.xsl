<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
	<xsl:output method="text" encoding="ISO-8859-1"/>
	<xsl:param name="scope">compiler</xsl:param>
	<!--xsl:include href="types.xsl"/>
<xsl:include href="includes.xsl"/-->

	<xsl:template match="version">
		<xsl:for-each select="field">
			<xsl:sort select="@order" data-type="number"/>
			<xsl:if test="position()>1">.</xsl:if>
			<xsl:apply-templates select="."/>
		</xsl:for-each>
	</xsl:template>

	<xsl:template name="valide">
		<xsl:param name="mode"/>
		<xsl:param name="vreq"/>
		<xsl:param name="version"/>
		<xsl:variable name="result">
			<xsl:choose>
				<xsl:when test="$mode = 'equal'">
					<xsl:if test="$vreq = $version">
						<xsl:value-of select="1"/>
					</xsl:if>
				</xsl:when>
				<xsl:when test="$mode = 'min'">
					<xsl:if test="$vreq &lt;= $version">
						<xsl:value-of select="1"/>
					</xsl:if>
				</xsl:when>
				<xsl:when test="$mode = 'max'">
					<xsl:if test="$vreq &gt;= $version">
						<xsl:value-of select="1"/>
					</xsl:if>
				</xsl:when>
				<xsl:when test="$mode = 'any'">
					<xsl:value-of select="1"/>
				</xsl:when>
			</xsl:choose>
		</xsl:variable>
		<xsl:choose>
			<xsl:when test="$result = '1'">
				<xsl:value-of select="true()"/>
			</xsl:when>
			<xsl:when test="$result != '1'">
				<xsl:value-of select="false()"/>
			</xsl:when>
		</xsl:choose>
	</xsl:template>
	
	<xsl:template match="project">
  Producto: <xsl:apply-templates select="artifactId"/>
  Version: <xsl:apply-templates select="version"/>
  Dependencias: 
		<xsl:for-each select="dependencies/dependency">
    Dependencia: <xsl:apply-templates select="artifactId"/> - Version: <xsl:apply-templates select="version"/>
		</xsl:for-each>

  Chequeo de Dependencias para <xsl:apply-templates select="artifactId"/>:
		<xsl:for-each select="dependencies/dependency">
			<xsl:if test="scope = $scope" >
				<xsl:variable name="pom" select="concat(path,'/pom.xml')"/>
				<xsl:variable name="pomDoc" select="document($pom)"/>
			<!--Chequeo de Dependencias de las Dependencias [<xsl:value-of select="$pom"/>]-->
				<xsl:apply-templates select="$pomDoc"/>
				<xsl:for-each select="version/field">
					<xsl:sort select="@order" data-type="number"/>
					<xsl:variable name="verDep" select="@order"/>
					<xsl:variable name="valideResult">
						<xsl:call-template name="valide">
							<xsl:with-param name="mode" select="@valid"/>
							<xsl:with-param name="vreq" select="."/>
							<xsl:with-param name="version" select="$pomDoc/project/version/field[@order=$verDep]"/>
						</xsl:call-template>
					</xsl:variable>
					<xsl:if test="'false' = $valideResult">
	  Error: la version de <xsl:apply-templates select="$pomDoc/project/artifactId"/> es incorrecta.
						<xsl:message terminate="yes">ERROR: Existen dependencias NO cumplidas.</xsl:message>
					</xsl:if>
				</xsl:for-each>
	  OK: la version de <xsl:apply-templates select="$pomDoc/project/artifactId"/> y de sus dependencias es correcta.
	  		</xsl:if>
		</xsl:for-each>
	</xsl:template>
	<!--xsl:variable name="promosVersions" select="document('version.xml')"/-->
</xsl:stylesheet>
