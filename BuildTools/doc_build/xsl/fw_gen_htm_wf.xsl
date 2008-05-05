<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="2.0">
<!--V1.0.0 20080202 -hdr- Generates html frames version from xml processed from tagged header files.-->
<!--V1.0.1 20080307 -hdr- Removed footer frame, added null version data templates to trap out default processing.-->
<!--V1.1.0 20080505 -hdr- Addded two new tags to support change history indents.-->
	<xsl:template match="/">
		<xsl:apply-templates/>
	</xsl:template>
	<xsl:template match="Manual">
	<xsl:apply-templates/>
		</xsl:template>
	<xsl:template match="VersionNumber"></xsl:template>
	<xsl:template match="VersionDate"></xsl:template>
<!-- Begin ancillary file generation.-->
<!-- Create master frameset document. Current version does not use a footer frame, but it can be added by modifying the frameset definition.-->
	<xsl:template match="FrameMaster">
		<xsl:result-document href="aa_000_frames.html">
		<HTML>
			<HEAD>
				<TITLE>Framewave</TITLE>
				<link rel="stylesheet" type="text/css" media="all" href="fw.css" />
				<script language="javascript">
					<xsl:comment>Begin if (window == top) top.location.href = "aa_000_frames.html"; //  End</xsl:comment>
				</script>
			</HEAD>
				<frameset rows="70,*">
					<frame src="aa_001_header.html" noresize="yes" />
						<frameset cols="200,*">
							<frame src="aa_003_toc.html" />
							<frame src="aa_005_intro.html" name="fw_content" marginwidth="10" marginheight="15" />
						</frameset>
					<noframes>
						<BODY>
							<P class="titlelarge">The Framewave Programming Reference is large and complex.</P>
							<P class="titlelarge">To take advantage of navigation features,</P>
							<P class="titlelarge">it is best to view it using browser frames.</P>
							<P class="titlelarge">For a no-frames version of the Programming Reference,</P>
							<P class="titlelarge">please click <A HREF="framewave_noframes.html">HERE</A>.</P>
						</BODY>
					</noframes>
				</frameset>
		</HTML>
		</xsl:result-document>
	</xsl:template>
<!-- Create header document -->
	<xsl:template match="HeaderMaster">
		<xsl:result-document href="aa_001_header.html">
		<HTML>
			<HEAD>
				<TITLE>Framewave</TITLE>
				<link rel="stylesheet" type="text/css" media="all" href="fw.css" />
				<script language="javascript">
					<xsl:comment>Begin if (window == top) top.location.href = "aa_000_frames.html"; //  End</xsl:comment>
				</script>
			</HEAD>
			<BODY leftmargin="0" topmargin="0" bgcolor="#ffffff" marginheight="0" marginwidth="0">
				<TABLE class="head">
					<TBODY>
						<TR>
							<TD><div align="left"><img src="fw_logo.gif" alt="Framewave" width="301" height="41" border="0" align="middle"></img></div></TD>
						</TR>
					</TBODY>
				</TABLE>
			</BODY>
		</HTML>
		</xsl:result-document>
	</xsl:template>
<!-- Create footer document. Not used in current version, could be used for additional navigation.
	<xsl:template match="FooterMaster">
		<xsl:result-document href="aa_002_footer.html">
		<HTML>
			<HEAD>
				<TITLE>Framewave</TITLE>
				<link rel="stylesheet" type="text/css" media="all" href="fw.css" />
				<script language="javascript">
					<xsl:comment>Begin if (window == top) top.location.href = "aa_000_frames.html"; //  End</xsl:comment>
				</script>
			</HEAD>
			<BODY leftmargin="0" topmargin="0" bgcolor="#ffffff" marginheight="0" marginwidth="0">
				<TABLE class="foot">
					<TR>
						<TD>Programming Reference</TD>
					</TR>
				</TABLE> 
			</BODY>
		</HTML>
		</xsl:result-document>
	</xsl:template>-->
<!-- Create navbar document -->
	<xsl:template match="NavBar">
		<xsl:result-document href="aa_003_toc.html">
		<HTML>
			<HEAD>
				<TITLE>Framewave</TITLE>
				<link rel="stylesheet" type="text/css" media="all" href="fw.css" />
				<script language="javascript">
					<xsl:comment>Begin if (window == top) top.location.href = "aa_000_frames.html"; //  End</xsl:comment>
				</script>
				<BASE target="fw_content"></BASE>
			</HEAD>
			<BODY leftmargin="0" topmargin="0" bgcolor="#ffffff" marginheight="0" marginwidth="0">
				<xsl:for-each select="/Manual/Section/FrontMatter/FrontSection/FrontHeading">
					<P class="toc1"><A class="nav" href="{FrontLink}.html#{FrontLink}" target="fw_content"><xsl:value-of select="FrontName"/></A></P>
				</xsl:for-each>
				<xsl:for-each select="/Manual/Section/Introduction/Heading1">
					<P class="toc1"><A class="nav" href="{H1Link}.html#{H1Link}" target="fw_content"><xsl:value-of select="H1Name"/></A></P>
				</xsl:for-each>
				<xsl:for-each select="/Manual/Section/Introduction/Heading2">
					<P class="toc2"><A class="nav" href="{/Manual/Section/Introduction/Heading1/H1Link}.html#{H2Link}" target="fw_content"><xsl:value-of select="H2Name"/></A></P>
				</xsl:for-each>
				<xsl:for-each select="/Manual/Section/Install/Heading1">
					<P class="toc1"><A class="nav" href="{H1Link}.html#{H1Link}" target="fw_content"><xsl:value-of select="H1Name"/></A></P>
				</xsl:for-each>
				<xsl:for-each select="/Manual/Section/Install/Heading2">
					<P class="toc2"><A class="nav" href="{/Manual/Section/Install/Heading1/H1Link}.html#{H2Link}" target="fw_content"><xsl:value-of select="H2Name"/></A></P>
				</xsl:for-each>
				<xsl:for-each select="/Manual/Section">
					<xsl:for-each select="SectionHeading">
						<P class="toc1"><A class="nav" href="{SectionLink}.html#{SectionLink}" target="fw_content"><xsl:value-of select="SectionName" /></A></P>
						<xsl:for-each select="following-sibling::Chapter">
							<xsl:for-each select="ChapterHeading">
								<P class="toc2"><A class="nav" href="{ChapterLink}.html#{ChapterLink}" target="fw_content"><xsl:value-of select="ChapterName" /></A></P>
							</xsl:for-each>
						<xsl:for-each select="following-sibling::Heading2">
							<P class="toc3"><A class="nav" href="{ChapterHeading/ChapterLink}.html#{H2Link}" target="fw_content"><xsl:value-of select="H2Name"/></A></P>
						</xsl:for-each>
						<xsl:for-each select="FunctionBlock">
							<xsl:for-each select="FunctionHeading">
								<P class="toc3"><A class="nav" href="{FunctionLink}.html#{FunctionLink}" target="fw_content"><xsl:value-of select="FunctionName" /></A></P>
							</xsl:for-each>
						</xsl:for-each>
					</xsl:for-each>
				</xsl:for-each>
			</xsl:for-each>
			</BODY>
		</HTML>
	</xsl:result-document>
	</xsl:template>
<!-- End ancillary file generation -->
<!-- Create index/title document -->
	<xsl:template match="TitlePage">
		<xsl:result-document href="aa_index.html">
		<HTML>
			<HEAD>
				<TITLE>Framewave</TITLE>
				<link rel="stylesheet" type="text/css" media="all" href="fw.css" />
				<script language="javascript">
					<xsl:comment>Begin if (window == top) top.location.href = "aa_000_frames.html"; //  End</xsl:comment>
				</script>
			</HEAD>
			<BODY leftmargin="0" topmargin="0" bgcolor="#ffffff" marginheight="0" marginwidth="0">
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
												<TR><TD><A class="titletext" href="aa_000_frames.html" target="">Begin Framewave</A></TD></TR>
												<TR><TD><A class="titletext" href="aa_007_tcd.html" target="">Copyright and Trademarks</A></TD></TR>
												<TR><TD><HR width="100%" size="1" class="hr"></HR></TD></TR>
											</TBODY>
										</TABLE>
									</DIV>
								</DIV>
							</TD>
						</TR>
					</TBODY>
				</TABLE>
			</BODY>
		</HTML>
		</xsl:result-document>
	</xsl:template>
<!-- Begin front matter file generation. Source is include file front.txt, added to the beginning of generated xml. -->
	<xsl:template match="FrontMatter">
	<xsl:apply-templates/>
	</xsl:template>    
	<xsl:template match="FrontSection">
	<xsl:result-document href="{FrontHeading/FrontLink}.html">
		<HTML>
			<HEAD>
				<TITLE><xsl:value-of select="FrontHeading/FrontName"/></TITLE>
				<link rel="stylesheet" type="text/css" media="all" href="fw.css" />
				<script language="javascript">
					<xsl:comment>Begin if (window == top) top.location.href = "aa_000_frames.html"; //  End</xsl:comment>
				</script>
			</HEAD>
			<BODY bgcolor="#ffffff">
				<xsl:apply-templates/>
			</BODY>
		</HTML>
		</xsl:result-document>
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
<!-- Create change history -->
	<xsl:template match="ChangeHistory">
		<TABLE class="fwtable" cellpadding="4" border="1"> 
			<THEAD align ="Center" valign="Top">  
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
					<TD><xsl:apply-templates select="ChangeDate"/></TD>
					<TD><xsl:apply-templates select="ChangeVersion"/></TD>
					<TD><xsl:apply-templates select="ChangeDescription"/>
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
<!-- End front matter file generation -->
<!-- Begin intro generation from source xml -->
	<xsl:template match="Introduction">
		<xsl:result-document href="{Heading1/H1Link}.html">
		<HTML>
			<HEAD>
				<TITLE><xsl:value-of select="Heading1/H1Name"/></TITLE>
				<link rel="stylesheet" type="text/css" media="all" href="fw.css" />
				<script language="javascript">
					<xsl:comment>Begin if (window == top) top.location.href = "aa_000_frames.html"; //  End</xsl:comment>
				</script>
			</HEAD>
			<BODY bgcolor="#ffffff">
				<xsl:apply-templates />
			</BODY>
		</HTML>
		</xsl:result-document>
	</xsl:template>
<!-- Create links to sections in introduction. -->
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
<!-- Create links to quick reference. -->
	<xsl:template match="RefLink">
		<xsl:for-each select="/Manual/Section/SectionHeading">
			<xsl:if test="SectionName='Function Quick Reference'">
				<P class="bodytext">
					The <A href="#{SectionLink}"><xsl:value-of select="SectionName" /></A> provides an index and snapshot view of function optimizations.
				</P>
			</xsl:if>
		</xsl:for-each>
	</xsl:template>
<!-- Begin install generation. Source is include file install.txt, added to the end of generated xml intro. -->  
	<xsl:template match="Install">
		<xsl:result-document href="{Heading1/H1Link}.html">
		<HTML>
			<HEAD>
				<TITLE><xsl:value-of select="Heading1/H1Name"/></TITLE>
				<link rel="stylesheet" type="text/css" media="all" href="fw.css" />
				<script language="javascript">
					<xsl:comment>Begin if (window == top) top.location.href = "aa_000_frames.html"; //  End</xsl:comment>
				</script>
			</HEAD>
			<BODY bgcolor="#ffffff">
				<xsl:apply-templates />
			</BODY>
		</HTML>
		</xsl:result-document>
	</xsl:template>
<!-- End intro and install generation -->
<!-- Begin library generation. Source is generated xml. -->
<!-- Create section documents -->
	<xsl:template match="Section">
	<xsl:result-document href="{SectionHeading/SectionLink}.html">
		<HTML>
			<HEAD>
				<TITLE><xsl:value-of select="SectionHeading/SectionName"/></TITLE>
				<link rel="stylesheet" type="text/css" media="all" href="fw.css" />
				<script language="javascript">
					<xsl:comment>Begin if (window == top) top.location.href = "aa_000_frames.html"; //  End</xsl:comment>
				</script>
			</HEAD>
			<BODY bgcolor="#ffffff">
				<xsl:apply-templates />
			</BODY>
		</HTML>
		</xsl:result-document>
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
<!-- Create links to chapters in section introduction. -->
	<xsl:template match="SectLinks">
		<UL class="bodytext">
			<xsl:for-each select="following-sibling::Chapter/ChapterHeading">
				<LI class="bodytext">The <A href="{ChapterLink}.html#{ChapterLink}" target="fw_content"><xsl:value-of select="ChapterName" /></A> <xsl:value-of select="../Paragraph/ChaptDesc" /> </LI>
			</xsl:for-each>
		</UL>
	</xsl:template>
	<xsl:template match="Chapter">
		<xsl:result-document href="{ChapterHeading/ChapterLink}.html">
		<HTML>
			<HEAD>
				<TITLE><xsl:value-of select="ChapterHeading/ChapterName"/></TITLE>
				<link rel="stylesheet" type="text/css" media="all" href="fw.css" />
				<script language="javascript">
					<xsl:comment>Begin if (window == top) top.location.href = "aa_000_frames.html"; //  End</xsl:comment>
				</script>
			</HEAD>
			<BODY bgcolor="#ffffff">
				<xsl:apply-templates />
				<!-- The following code distinguishes basic concepts chapters from chapters that contain function definitions and adds function links to the chapter title page.-->
				<xsl:choose>
					<xsl:when test="ChapterHeading/ChapterName='Basic Concepts'">
					</xsl:when>
					<xsl:otherwise>
						<P class="bodytext">This chapter contains the following function definitions.</P>
					</xsl:otherwise>
				</xsl:choose>
				<P class="bodytext"><xsl:for-each select="FunctionBlock/FunctionHeading"> |<A href="{FunctionLink}.html#{FunctionLink}"><xsl:value-of select="FunctionName" /></A>| </xsl:for-each></P>
			</BODY>
		</HTML>
		</xsl:result-document>
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
<!-- Function block structure is very tightly defined -->
	<xsl:template match="FunctionBlock">
		<xsl:result-document href="{FunctionHeading/FunctionLink}.html">
		<HTML>
			<HEAD>
				<TITLE><xsl:value-of select="FunctionHeading/FunctionName"/></TITLE>
				<link rel="stylesheet" type="text/css" media="all" href="fw.css" />
				<script language="javascript">
					<xsl:comment>Begin if (window == top) top.location.href = "aa_000_frames.html"; //  End</xsl:comment>
				</script>
			</HEAD>
			<BODY bgcolor="#ffffff">
				<xsl:apply-templates select="FunctionHeading"/>
				<xsl:apply-templates select="ShortDescription"/>
				<xsl:apply-templates select="SupportedTechnologies"/>
				<xsl:apply-templates select="FunctionDeclarationList"/>
				<xsl:apply-templates select="ParameterDefinitionList"/>
				<xsl:apply-templates select="DetailedDescription"/>
			</BODY>
		</HTML>
		</xsl:result-document>
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
				<TR align="left" valign="Top">
					<TD><xsl:apply-templates select="ReturnType"/></TD>
					<TD>&#160;</TD>
					<TD><xsl:apply-templates select="FunctionName"/></TD>
					<TD><B>(</B></TD>
					<TD>
						<xsl:for-each select="ParameterList">
							<xsl:for-each select="Parameter"><xsl:apply-templates select="Type"/>&#160;<xsl:apply-templates select="ParameterName"/>
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
					<TD><xsl:apply-templates select="Term"/></TD>
					<TD>&#160;</TD>
					<TD><xsl:apply-templates select="Definition"/></TD>
				</TR>
			</xsl:for-each>
		</TABLE>
	</xsl:template>
<!-- End library generation. Following elements are general-use definitions for preceding sections. -->    
	<xsl:template match="Blank"></xsl:template>
	<xsl:template match="ReturnType"><B><xsl:value-of select="."/></B></xsl:template>
	<xsl:template match="FunctionName"><B><xsl:value-of select="."/></B></xsl:template>
	<xsl:template match="Type"><B><xsl:value-of select="."/></B></xsl:template>
	<xsl:template match="ParameterName"><I><xsl:value-of select="."/></I></xsl:template>
	<xsl:template match="Parameter"><I><xsl:value-of select="."/></I></xsl:template>
	<xsl:template match="Constant"><B><xsl:value-of select="."/></B></xsl:template>
	<xsl:template match="DataType"><B><xsl:value-of select="."/></B></xsl:template>
	<xsl:template match="ReturnValue"><xsl:apply-templates/></xsl:template>
	<xsl:template match="Term"><xsl:apply-templates /></xsl:template>
	<xsl:template match="Definition"><xsl:apply-templates/></xsl:template>
	<xsl:template match="Text"><P class="bodytext"><xsl:apply-templates /></P></xsl:template>
	<xsl:template match="Pseudo"><PRE class="desctext"><xsl:value-of select="."/></PRE></xsl:template>
	<xsl:template match="Paragraph"><P class="bodytext"><xsl:apply-templates /></P></xsl:template>
	<xsl:template match="ParagraphIndent"><P class="bodyindent"><xsl:apply-templates /></P></xsl:template>
	<xsl:template match="Note"><P class="bodytext"><B><xsl:text>Note:&#160;</xsl:text></B><xsl:apply-templates /></P></xsl:template>
	<xsl:template match="Bold"><B><xsl:value-of select="." /></B></xsl:template>
	<xsl:template match="Italic"><I><xsl:apply-templates /></I></xsl:template>
	<xsl:template match="sub"><sub><xsl:value-of select="."/></sub></xsl:template>
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
<!-- Begin quick reference generation. Source is generated xml. -->    
	<xsl:template match="FunctionQuickReference">
			<DIV align="center">
				<TABLE class="quick_ref"> 
					<TR class="qr_row">
						<xsl:for-each select="QRColHead">
							<TD class="qr_col_head"><B><xsl:value-of select="."/></B></TD>
						</xsl:for-each>
					</TR>	
					<xsl:for-each select="QRRow">
						<TR class="qr_row">
							<TD class="qr_cell"><B><xsl:apply-templates select="QRFunction" /></B></TD>
							<TD class="qr_cell"><B><xsl:apply-templates select="QRChapter" /></B></TD>
							<TD class="qr_cell"><B><xsl:apply-templates select="QRSection" /></B></TD>
							<xsl:for-each select="QRCell">
								<TD class="qr_cell"><xsl:value-of select="."/></TD>
							</xsl:for-each>
						</TR>
					</xsl:for-each>
				</TABLE>
			</DIV>
	</xsl:template>
	<xsl:template match="QRFunction">
		<A class="nav" href="{QRFunctionLink}.html#{QRFunctionLink}"><xsl:value-of select="QRFunctionName" /></A>
	</xsl:template>
	<xsl:template match="QRChapter">
		<A class="nav" href="{QRChapterLink}.html#{QRChapterLink}"><xsl:value-of select="QRChapterName" /></A>
	</xsl:template>
	<xsl:template match="QRSection">
		<A class="nav" href="{QRSectionLink}.html#{QRSectionLink}"><xsl:value-of select="QRSectionName" /></A>
	</xsl:template>
<!-- End quick reference generation -->
</xsl:stylesheet>