<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="2.0">
<!--V1.0.0 20080202 -hdr- generates html noframes version from xml processed from tagged header files.-->
<!--V1.0.1 20080307 -hdr- Added null version data templates to trap out default processing.-->
<!--V1.1.0 20080505 -hdr- updated to include two new tags for change history indents.-->
	<xsl:template match="/">
		<xsl:apply-templates/>
	</xsl:template>
	<xsl:template match="VersionNumber"></xsl:template>
	<xsl:template match="VersionDate"></xsl:template>
	<xsl:template match="Manual">
	<xsl:result-document href="framewave_noframes.html">
		<HTML>
			<HEAD>
				<TITLE>Framewave</TITLE>
				<link rel="stylesheet" type="text/css" media="all" href="fw_nf.css" />
			</HEAD>
				<BODY leftmargin="0" topmargin="0" bgcolor="#ffffff" marginheight="0" marginwidth="0">	
					<xsl:apply-templates/>
				</BODY>
		</HTML>
	</xsl:result-document>	
	</xsl:template>
	<xsl:template match="TitlePage">
				<TABLE width="100%" height="100%" align="center" class="title_page">
					<TBODY>
						<TR>
							<TD>
								<DIV>
									<DIV align="center">
										<TABLE width="300" border="0" cellspacing="10" cellpadding="0">
											<TBODY>
												<TR>
													<TD>
														<div align="left"><img src="fw_logo.gif" alt="Framewave" width="301" height="41" border="0" align="middle"></img></div>
														<P class="titletext">Version&#160;<xsl:value-of select="/Manual/Section/VersionNumber" />&#160;|&#160;<xsl:value-of select="/Manual/Section/VersionDate" /></P>
													</TD>
												</TR>
												<TR><TD><P class="titletext">&#160;</P></TD></TR>
												<TR><TD><P class="titlelarge">Programming Reference</P></TD></TR>
												<TR><TD><P class="titletext">&#160;</P></TD></TR>
												<TR><TD><A class="titletext" href="#aa_007_tcd">Copyright and Trademarks</A></TD></TR>
												<TR><TD><A class="titletext" href="#aa_000_toc">Table of Contents</A></TD></TR>
												<TR><TD><A class="titletext" href="#fw_section_060">Function Quick Reference</A></TD></TR>
												<TR><TD><HR width="100%" size="1" class="hr"></HR></TD></TR>
												<TR><TD class="titlelarge"> This is the NOFRAMES version</TD></TR>
												<TR><TD class="titlelarge"> Click <A href="index.html">HERE</A> for the Frames version</TD></TR>
											</TBODY>
										</TABLE>
									</DIV>
								</DIV>
							</TD>
						</TR>
					</TBODY>
				</TABLE>
	</xsl:template>
		<xsl:template match="TOC">
			<H1 class="GenericHead1">Table of Contents<A name="aa_000_toc"></A></H1>
				<xsl:for-each select="/Manual/Section/FrontMatter/FrontSection/FrontHeading">
					<P class="toc1">
						<A class="nav" href="#{FrontLink}"><xsl:value-of select="FrontName"/></A>
					</P>
				</xsl:for-each>
				<xsl:for-each select="/Manual/Section/Introduction/Heading1">
					<P class="toc1">
						<A class="nav" href="#{H1Link}"><xsl:value-of select="H1Name"/></A>
					</P>
				</xsl:for-each>
				<xsl:for-each select="/Manual/Section/Introduction/Heading2">
					<P class="toc2">
						<A class="nav" href="#{H2Link}"><xsl:value-of select="H2Name"/></A>
					</P>
				</xsl:for-each>	
				<xsl:for-each select="/Manual/Section/Install/Heading1">
					<P class="toc1">
						<A class="nav" href="#{H1Link}"><xsl:value-of select="H1Name"/></A>
					</P>
				</xsl:for-each>
				<xsl:for-each select="/Manual/Section/Install/Heading2">
					<P class="toc2">
						<A class="nav" href="#{H2Link}"><xsl:value-of select="H2Name"/></A>
					</P>
				</xsl:for-each>
				<xsl:for-each select="/Manual/Section">
					<xsl:for-each select="SectionHeading">		
						<P class="toc1">
							<A class="nav" href="#{SectionLink}"><xsl:value-of select="SectionName" /></A>
						</P>
						<xsl:for-each select="following-sibling::Chapter">
							<xsl:for-each select="ChapterHeading">
								<P class="toc2">
									<A class="nav" href="#{ChapterLink}"><xsl:value-of select="ChapterName" /></A>
								</P>
							</xsl:for-each>
						<xsl:for-each select="following-sibling::Heading2">
							<P class="toc3">
								<A class="nav" href="#{H2Link}"><xsl:value-of select="H2Name"/></A>
							</P>
						</xsl:for-each>
						<xsl:for-each select="FunctionBlock">
							<xsl:for-each select="FunctionHeading">				
								<P class="toc3">
									<A class="nav" href="#{FunctionLink}"><xsl:value-of select="FunctionName" /></A>
								</P>
							</xsl:for-each>
						</xsl:for-each>
					</xsl:for-each>
				</xsl:for-each>
			</xsl:for-each>
	</xsl:template>
	<xsl:template match="FrontMatter">
		<xsl:apply-templates/>
	</xsl:template>
	<xsl:template match="FrontSection">
				<xsl:apply-templates/>
	</xsl:template>  
	<xsl:template match="FrontHeading">
		<H1 class="GenericHead1">
			<xsl:value-of select="FrontName"/>
			<xsl:element name="a">
				<xsl:attribute name="name">
					<xsl:value-of select="FrontLink"/>
				</xsl:attribute>
			</xsl:element>
		</H1>
	</xsl:template>
	<xsl:template match="ChangeHistory">
		<TABLE class="fwtable" cellpadding="4" border="1"> 
			<THEAD align ="Left" valign="Top">  
				<TH>
					<P>Date</P>
				</TH>
				<TH>
					<P>Version</P>
				</TH>
			 	<TH>
					<P>Description of Change</P>
				</TH>
			</THEAD>
			<TBODY>
				<xsl:for-each select="ChangeEntry">
					<TR align ="Left" valign="Top">  
						<TD>
							<xsl:apply-templates select="ChangeDate"/>
						</TD>
						<TD>
							<xsl:apply-templates select="ChangeVersion"/>
						</TD>
						<TD>
							<xsl:apply-templates select="ChangeDescription"/>
							<xsl:for-each select="ChangeDescription/ChangeItem">
								<xsl:apply-templates select="ChangeItem"/>
								<xsl:for-each select="ChangeDescription/ChangeSubItem">
									<xsl:apply-templates select="ChangeSubItem"/>
								</xsl:for-each>
							</xsl:for-each>
						</TD>
					</TR>
				</xsl:for-each>
			</TBODY>
		</TABLE>
	</xsl:template>
	<xsl:template match="ChangeDate">
		<P class="changetext"><xsl:apply-templates /></P>
	</xsl:template>
	<xsl:template match="ChangeVersion">
		<P class="changetext"><xsl:apply-templates /></P>
	</xsl:template>
	<xsl:template match="ChangeDescription">
		<P class="changetext"><xsl:apply-templates /></P>
	</xsl:template>
	<xsl:template match="ChangeItem">
		<P class="changeitem"><xsl:apply-templates /></P>
	</xsl:template>
	<xsl:template match="ChangeSubItem">
		<P class="changesubitem"><xsl:apply-templates /></P>
	</xsl:template>
	<xsl:template match="Introduction">
		<xsl:apply-templates />
	</xsl:template>
	<xsl:template match="IntroLinks">
		<UL class="bodytext">
			<xsl:for-each select="/Manual/Section/SectionHeading">
			<xsl:choose>
				<xsl:when test="SectionName='Function Quick Reference'">
					<xsl:apply-templates select ="Blank" />
				</xsl:when>
				<xsl:otherwise>
					<LI class="bodytext">The <A href="{SectionLink}.html#{SectionLink}" target="fw_content"><xsl:value-of select="SectionName" /></A> <xsl:value-of select="../Paragraph/SectDesc" /></LI>
				</xsl:otherwise>
			</xsl:choose>
			</xsl:for-each>
		</UL>
	</xsl:template>
	<xsl:template match="RefLink">
		<xsl:for-each select="/Manual/Section/SectionHeading">
			<xsl:if test="SectionName='Function Quick Reference'">
				<P class="bodytext">
					The <A href="#{SectionLink}"><xsl:value-of select="SectionName" /></A> provides an index and snapshot view of function optimizations.
				</P>
			</xsl:if>
		</xsl:for-each>
	</xsl:template>
	<xsl:template match="Install">
		<xsl:apply-templates />
	</xsl:template>
	<xsl:template match="Section">
		<xsl:apply-templates />
	</xsl:template>
	<xsl:template match="SectionHeading">
		<H1 class="SectionHead">
			<xsl:value-of select="SectionName"/>
			<xsl:element name="a">
				<xsl:attribute name="name">
					<xsl:value-of select="SectionLink"/>
				</xsl:attribute>
			</xsl:element>
		</H1>
	</xsl:template>
	<xsl:template match="SectLinks">
		<UL class="bodytext">
			<xsl:for-each select="following-sibling::Chapter/ChapterHeading">
				<LI class="bodytext">
					The <A href="#{ChapterLink}"><xsl:value-of select="ChapterName" /></A> <xsl:value-of select="../Paragraph/ChaptDesc" /> 
				</LI>
			</xsl:for-each>
		</UL>
	</xsl:template>
	<xsl:template match="Chapter">
		<xsl:apply-templates />
	</xsl:template>
	<xsl:template match="ChapterHeading">
		<H2 class="ChapterHead">
			<xsl:value-of select="ChapterName"/>
			<xsl:element name="a">
				<xsl:attribute name="name">
					<xsl:value-of select="ChapterLink"/>
				</xsl:attribute>
			</xsl:element>
		</H2>
	</xsl:template>
	<xsl:template match="FunctionBlock">
		<xsl:apply-templates select="FunctionHeading"/>
		<xsl:apply-templates select="ShortDescription"/>
		<xsl:apply-templates select="SupportedTechnologies"/>
		<xsl:apply-templates select="FunctionDeclarationList"/>
		<xsl:apply-templates select="ParameterDefinitionList"/>
		<xsl:apply-templates select="DetailedDescription"/>
	</xsl:template>
	<xsl:template match="FunctionHeading">
		<H3 class="FunctionHead">
			<xsl:value-of select="FunctionName"/>
			<xsl:element name="a">
				<xsl:attribute name="name">
					<xsl:value-of select="FunctionLink"/>
				</xsl:attribute>
			</xsl:element>
		</H3>
	</xsl:template>
	<xsl:template match="ShortDescription">
		<P class="bodytext"><xsl:apply-templates/></P>
	</xsl:template>
	<xsl:template match="SupportedTechnologies">
		<H4 class="FunctionSubhead">Supported Technologies</H4>
		<P class="bodytext"><I><xsl:value-of select="SupportedTechnologiesDefinition"/></I></P>
	</xsl:template>
	<xsl:template match="FunctionDeclarationList">
		<H4 class="FunctionSubhead">Synopsis</H4>
		<TABLE class="fwtable" border="0"> 
			<xsl:for-each select="FunctionDeclaration">
				<TR  align="left" valign="Top">
					<TD><xsl:apply-templates select="ReturnType"/></TD>
					<TD>&#160;</TD>
					<TD><xsl:apply-templates select="FunctionName"/></TD>
					<TD><B>(</B></TD>
					<TD>
						<xsl:for-each select="ParameterList">
							<xsl:for-each select="Parameter">
								<xsl:apply-templates select="Type"/>&#160;
								<xsl:apply-templates select="ParameterName"/>			
								<xsl:if test="position()!=last()">,&#160;</xsl:if>		
							</xsl:for-each>
						</xsl:for-each>
						<B>&#160;);</B>
					</TD>
				</TR>
			</xsl:for-each>
		</TABLE>
	</xsl:template>
	<xsl:template match="ParameterDefinitionList">
		<H4 class="FunctionSubhead">Parameters</H4>
		<xsl:apply-templates select="DefinitionList"/>
	</xsl:template>
	<xsl:template match="DetailedDescription">
		<H4 class="FunctionSubhead">Description</H4>
		<xsl:apply-templates />			
	</xsl:template>
	<xsl:template match="ReturnedValueList">
		<H4 class="FunctionSubhead">Return Values</H4>
		<xsl:apply-templates select="DefinitionList"/>
	</xsl:template>
	<xsl:template match="DefinitionList">
		<TABLE class="fwtable" border="0"> 
			<xsl:for-each select="DefinitionEntry">
			<TR align ="Left" valign="Top">  
				<TD>
					<xsl:apply-templates select="Term"/>
				</TD>
				<TD>&#160;</TD>
				<TD>
					<xsl:apply-templates select="Definition"/>
				</TD>
			</TR>
			</xsl:for-each>
		</TABLE>
	</xsl:template>
	<xsl:template match="Blank"></xsl:template>
	<xsl:template match="ReturnType">
		<B><xsl:value-of select="."/></B>
	</xsl:template>	
	<xsl:template match="FunctionName">
		<B><xsl:value-of select="."/></B>
	</xsl:template>
	<xsl:template match="Type">
		<B><xsl:value-of select="."/></B>
	</xsl:template>
	<xsl:template match="ParameterName">
		<I><xsl:value-of select="."/></I>
	</xsl:template>
	<xsl:template match="Parameter">
	 	<I><xsl:value-of select="."/></I>
	</xsl:template>
	<xsl:template match="Constant">
		<B><xsl:value-of select="."/></B>
	</xsl:template>
	<xsl:template match="DataType">
		<B><xsl:value-of select="."/></B>
	</xsl:template>
	<xsl:template match="ReturnValue">
		<xsl:apply-templates />
	</xsl:template>
	<xsl:template match="Text">
		<P class="bodytext"><xsl:apply-templates /></P>
	</xsl:template>
	<xsl:template match="Pseudo">
		<PRE class="desctext"><xsl:value-of select="."/></PRE>
	</xsl:template>
	<xsl:template match="Paragraph">
		<P class="bodytext"><xsl:apply-templates /></P>
	</xsl:template>
	<xsl:template match="ParagraphIndent">
		<P class="bodyindent"><xsl:apply-templates /></P>
	</xsl:template>
	<xsl:template match="Note">
		<P class="bodytext"><B><xsl:text>Note:&#160;</xsl:text></B><xsl:apply-templates /></P>
	</xsl:template>
	<xsl:template match="Bold">
		<B><xsl:value-of select="." /></B>
	</xsl:template>
	<xsl:template match="Italic">
		<I><xsl:apply-templates /></I>
	</xsl:template>
	<xsl:template match="sub">
		<sub><xsl:value-of select="."/></sub>
	</xsl:template>
	<xsl:template match="Heading1">
		<H1 class="GenericHead1">
		<xsl:value-of select="H1Name"/>
			<xsl:element name="a">
				<xsl:attribute name="name">
					<xsl:value-of select="H1Link"/>
				</xsl:attribute>
			</xsl:element>
		</H1>
	</xsl:template>
	<xsl:template match="Heading2">
		<H2 class="GenericHead2">
			<xsl:value-of select="H2Name"/>
			<xsl:element name="a">
				<xsl:attribute name="name">
					<xsl:value-of select="H2Link"/>
				</xsl:attribute>
			</xsl:element>
		</H2>
	</xsl:template>
	<xsl:template match="Heading3">
		<H3 class="GenericHead3">
			<xsl:value-of select="H3Name"/>
			<xsl:element name="a">
				<xsl:attribute name="name">
					<xsl:value-of select="H3Link"/>
				</xsl:attribute>
		</xsl:element>
		</H3>
	</xsl:template>
	<xsl:template match="Heading4">
		<H4 class="GenericHead4">
			<xsl:value-of select="."/>
		</H4>
	</xsl:template>
	<xsl:template match="BulletedList">
		<UL class="bodytext">
			<xsl:for-each select="Bullet">
				<LI><xsl:apply-templates/></LI> 
			</xsl:for-each>
		</UL>
	</xsl:template>
	<xsl:template match="NumberedList">
		<OL class="bodytext">
			<xsl:for-each select="NumberedItem">
				<LI><xsl:apply-templates /></LI> 
			</xsl:for-each>
		</OL>
	</xsl:template>
	<xsl:template match="hlink">
		<A href="{uri}" target="{target}"><xsl:value-of select="desc" /></A>
	</xsl:template>
	<xsl:template match="Figure">
		<P class="figure">
			<xsl:element name="IMG">
				<xsl:attribute name="SRC">
					<xsl:value-of select="SRC"/>
				</xsl:attribute>
				<xsl:attribute name="ALT">
					<xsl:value-of select="ALT"/>
				</xsl:attribute>
				<xsl:attribute name="BORDER">
					<xsl:value-of select="BORDER"/>
				</xsl:attribute>
				<xsl:if test="HEIGHT">
					<xsl:attribute name="HEIGHT">
						<xsl:value-of select="HEIGHT"/>
					</xsl:attribute>
				</xsl:if>
				<xsl:if test="WIDTH">
					<xsl:attribute name="WIDTH">
						<xsl:value-of select="WIDTH"/>
					</xsl:attribute>
				</xsl:if>
			</xsl:element>
		</P>
	</xsl:template>
	<xsl:template match="Caption">
		<P class="caption">
			<xsl:value-of select="." />
		</P>
	</xsl:template>
	<xsl:template match="FunctionQuickReference">
			<DIV align="center">
				<TABLE class="quick_ref"> 
					<TR class="qr_row">
						<xsl:for-each select="QRColHead">
							<TD class="qr_col_head">
								<B><xsl:value-of select="."/></B>
							</TD>
						</xsl:for-each>
					</TR>
					<xsl:for-each select="QRRow">
						<TR class="qr_row">
							<TD class="qr_cell">
								<B><xsl:apply-templates select="QRFunction" /></B>	
							</TD>
							<TD class="qr_cell">
								<B><xsl:apply-templates select="QRChapter" /></B>
							</TD>
							<TD class="qr_cell">
								<B><xsl:apply-templates select="QRSection" /></B>
							</TD>
							<xsl:for-each select="QRCell">
								<TD class="qr_cell"><xsl:value-of select="."/></TD>
							</xsl:for-each>
						</TR>
					</xsl:for-each>
				</TABLE>
			</DIV>
	</xsl:template>
	<xsl:template match="QRFunction">
		<A  class="nav" href="#{QRFunctionLink}"><xsl:value-of select="QRFunctionName" /></A>
	</xsl:template>
	<xsl:template match="QRChapter">
		<A  class="nav" href="#{QRChapterLink}"><xsl:value-of select="QRChapterName" /></A>
	</xsl:template>
	<xsl:template match="QRSection">
		<A  class="nav" href="#{QRSectionLink}"><xsl:value-of select="QRSectionName" /></A>		
	</xsl:template>
</xsl:stylesheet>  