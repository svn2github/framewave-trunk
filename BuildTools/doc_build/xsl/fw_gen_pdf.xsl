<?xml version="1.0"?>

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns="http://www.w3.org/1999/XSL/Format"
                version="2.0">
<!--working version of xsl-fo to generate pdf from sml source-->
<xsl:template match="/">
  <root font-size="16pt" id="{generate-id(/)}">
    <layout-master-set>
      <simple-page-master master-name="page_odd"
       page-height="11in" page-width="8.5in"
       margin-top=".75in" margin-bottom=".75in"
       margin-left=".75in" margin-right=".75in">
        <region-body region-name="page_body"
                     margin-top=".5in" 
                     margin-bottom=".5in"/>
        <region-before region-name="before_odd"
                       extent=".5in"/>
        <region-after region-name="after_odd"
                      extent=".5in"/>
      </simple-page-master>
      <simple-page-master master-name="page_even"
       page-height="11in" page-width="8.5in"
       margin-top=".75in" margin-bottom=".75in"
       margin-left=".75in" margin-right=".75in">
        <region-body region-name="page_body"
                     margin-top=".5in" 
                     margin-bottom=".5in"/>
        <region-before region-name="before_even"
                       extent=".5in"/>
        <region-after region-name="after_even"
                      extent=".5in"/>
      </simple-page-master>
      <page-sequence-master master-name="page">
        <repeatable-page-master-alternatives>
          <conditional-page-master-reference 
            master-reference="page_odd" odd-or-even="odd"/>
          <conditional-page-master-reference 
            master-reference="page_even" odd-or-even="even"/>
        </repeatable-page-master-alternatives>
      </page-sequence-master>
    </layout-master-set>

    <page-sequence master-reference="page">
      <title><xsl:value-of select="doc/title"/></title>
        <static-content flow-name="before_odd">
          <block text-align-last="end">
            <retrieve-marker retrieve-class-name="chapname"/>
          </block>
        </static-content>
        <static-content flow-name="before_even">
          <block>
            <retrieve-marker retrieve-class-name="chapname"/>
          </block>
        </static-content>
        <static-content flow-name="after_odd">
          <table>
            <table-column column-width="proportional-column-width(35)"/>
            <table-column column-width="proportional-column-width(30)"/>
            <table-column column-width="proportional-column-width(35)"/>
            <table-body>
              <table-row>
                <table-cell column-number="1">
                  <block>
                    <retrieve-marker 
                          retrieve-class-name="docname"/></block>
                </table-cell>
                <table-cell column-number="2">
                  <block text-align="center">Page <page-number/>
                    <xsl:text> of </xsl:text>
                    <page-number-citation-last
                              ref-id="{generate-id(/)}"/></block>
              </table-cell>
              </table-row>
            </table-body>
          </table>
        </static-content>
        <static-content flow-name="after_even">
          <table>
            <table-column column-width="proportional-column-width(35)"/>
            <table-column column-width="proportional-column-width(30)"/>
            <table-column column-width="proportional-column-width(35)"/>
            <table-body>
              <table-row>
                <table-cell column-number="2">
                  <block text-align="center">Page <page-number/>
                    <xsl:text> of </xsl:text>
                    <page-number-citation-last 
                              ref-id="{generate-id(/)}"/></block>
                </table-cell>
                <table-cell column-number="3">
                  <block text-align="end">
                    <retrieve-marker 
                          retrieve-class-name="docname"/></block>
                </table-cell>
              </table-row>
            </table-body>
          </table>
        </static-content>
        
      <flow flow-name="page_body">
        <block text-align="center" font-size="250%"
               font-weight="bold">
          <xsl:value-of select="doc/title"/>
        </block>
        <block line-height="3pt" space-before="1.5cm"
               text-align="center">
          <leader leader-pattern="rule" leader-length="80%"/>
          <block/>
          <leader leader-pattern="rule" leader-length="80%"/>
        </block>
        <block text-align="center" space-before="1.5cm" 
               space-after="1em" font-size="150%">
          <xsl:text>Table of Contents</xsl:text>
        </block>

        <xsl:for-each select="doc/chapter">
          <block start-indent="1cm" end-indent="1cm" 
                 text-align-last="justify">
            <basic-link internal-destination="{generate-id(.)}">
              <xsl:value-of select="title"/>
            </basic-link>
            <xsl:text> </xsl:text>
            <leader leader-pattern="dots"/>
            <xsl:text> </xsl:text>
            <basic-link internal-destination="{generate-id(.)}">
              <page-number-citation ref-id="{generate-id(.)}"/>
            </basic-link>
          </block>
        </xsl:for-each>
                                <!--an entry for the last page-->
        <block space-before="2.5em" 
               start-indent="1cm" end-indent="1cm" 
               text-align-last="justify">
          <xsl:text>Last page of book </xsl:text>
          <leader leader-pattern="dots"/>
          <xsl:text> </xsl:text>
          <page-number-citation-last ref-id="{generate-id(/)}"/>
        </block>
        <block break-after="odd-page"/>
                                                  <!--chapters-->
        <xsl:for-each select="/doc/chapter">
          <block font-size="150%" break-before="page"
                 id="{generate-id(.)}">
                    <!--dynamic content definition before flow-->
            <marker marker-class-name="docname">
              <xsl:value-of select="/doc/title"/>
            </marker>
            <marker marker-class-name="chapname">
              <xsl:value-of select="title"/>
            </marker>
            <xsl:value-of select="title"/>
          </block>
          <xsl:apply-templates/>
        </xsl:for-each>
      </flow>
    </page-sequence>
  </root>
</xsl:template>

<xsl:template match="title"/>            <!--already displayed-->

<xsl:template match="para">             <!--unordered elements-->
  <block space-before="1em">
    <xsl:apply-templates/></block>
</xsl:template>

<xsl:template match="bold">
  <inline font-weight="bold"><xsl:apply-templates/></inline>
</xsl:template>

<xsl:template match="italic">
  <inline font-style="italic"><xsl:apply-templates/></inline>
</xsl:template>

<xsl:template name="do-a-list"><!--common handling of a list out of context-->
  <list-block provisional-distance-between-starts="2cm"
              provisional-label-separation=".5cm">
    <xsl:if test="not(parent::ulist|parent::olist)">
      <xsl:attribute name="space-before">1em</xsl:attribute>
    </xsl:if>
    <xsl:apply-templates/>
  </list-block>
</xsl:template>

<xsl:template match="ulist|olist">
  <xsl:choose>    <!--must go into a list item if in another's list of items-->
    <xsl:when test="parent::ulist|parent::olist">
      <list-item>
        <list-item-label end-indent="label-end()">
          <block/>
        </list-item-label>
        <list-item-body start-indent="body-start()">
          <xsl:call-template name="do-a-list"/>
        </list-item-body>
      </list-item>
    </xsl:when>
    <xsl:otherwise>                    <!--just use list as it is in chapter-->
      <xsl:call-template name="do-a-list"/>
    </xsl:otherwise>
  </xsl:choose>            
</xsl:template>

<xsl:template match="li">
  <list-item relative-align="baseline">
    <list-item-label end-indent="label-end()">
      <block text-align="end">
        <xsl:choose>
          <xsl:when test="parent::ulist">-</xsl:when>          <!--unordered-->
          <xsl:otherwise><xsl:number/></xsl:otherwise>           <!--ordered-->
        </xsl:choose>
      </block>
    </list-item-label>
    <list-item-body start-indent="body-start()">
      <block>
        <xsl:apply-templates/>
      </block>
    </list-item-body>
  </list-item>
</xsl:template>

<xsl:template match="suppress"/>

</xsl:stylesheet>
